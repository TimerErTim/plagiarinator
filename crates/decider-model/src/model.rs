use burn::{
    Tensor, config::Config, module::Module, nn::{
        Dropout, DropoutConfig, Embedding, EmbeddingConfig, LayerNorm, LayerNormConfig, Linear,
        LinearConfig, SwiGlu, SwiGluConfig,
    }, prelude::Backend, tensor::{
        Int, activation::{relu, sigmoid, silu}
    }
};

use crate::{
    data::Graph,
    layers::{GraphConvolution, GraphConvolutionConfig, GraphDiffPool, GraphDiffPoolConfig},
};

#[derive(Config, Debug)]
pub struct PlagiarismDeciderConfig {
    num_classes: usize,
    embedding_size: usize,
    comparator_size: usize,
    dropout_rate: f64,
    // The number of output features for each layer
    layers: Vec<PlagiarismDeciderLayerConfig>,
}

#[derive(Config, Debug)]
pub struct PlagiarismDeciderLayerConfig {
    pub output_features: usize,
    pub num_clusters: usize,
    #[config(default = 0)]
    pub pre_aggregations: usize,
}

impl PlagiarismDeciderConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> PlagiarismDecider<B> {
        let mut compression_layers: Vec<(Vec<GraphConvolutionConfig>, GraphDiffPoolConfig, LayerNormConfig)> =
            Vec::with_capacity(self.layers.len());
        for layer_config in &self.layers {
            let prev_layer_features = if let Some(prev_layer) = compression_layers.last() {
                prev_layer.1.output_features
            } else {
                self.embedding_size
            };
            compression_layers.push((
                (0..layer_config.pre_aggregations).map(|_| GraphConvolutionConfig::new(prev_layer_features, prev_layer_features)).collect(),
                GraphDiffPoolConfig::new(
                    prev_layer_features,
                    layer_config.output_features,
                    layer_config.num_clusters,
                ),
                LayerNormConfig::new(layer_config.output_features),
            ));
        }
        let last_layer_features = compression_layers
            .last()
            .map(|(aggregate, pool, _norm)| pool.output_features)
            .unwrap_or(self.embedding_size);

        PlagiarismDecider {
            embedding: EmbeddingConfig::new(self.num_classes, self.embedding_size).init(device),
            compression_layers: compression_layers
                .into_iter()
                .map(|(aggregate_configs, pool_config, norm_config)| {
                    (
                        aggregate_configs.into_iter().map(|config| config.init(device)).collect(),
                        pool_config.init(device),
                        norm_config.init(device)
                    )
                })
                .collect(),
            dropout: DropoutConfig::new(self.dropout_rate).init(),
            common_gate: LinearConfig::new(last_layer_features, last_layer_features).init(device),
            distance_weights: LinearConfig::new(last_layer_features, 1).with_bias(false).init(device),
        }
    }
}

#[derive(Module, Debug)]
pub struct PlagiarismDecider<B: Backend> {
    pub embedding: Embedding<B>,
    compression_layers: Vec<(Vec<GraphConvolution<B>>, GraphDiffPool<B>, LayerNorm<B>)>,
    dropout: Dropout,
    common_gate: Linear<B>,
    distance_weights: Linear<B>,
}

impl<B: Backend> PlagiarismDecider<B> {
    pub fn embed_nodes(&self, nodes: Tensor<B, 2, Int>) -> Tensor<B, 2> {
        self.embedding
            .forward(nodes.clone().swap_dims(0, 1))
            .squeeze_dim(0)
    }

    pub fn embed_graph(&self, graph: Graph<B, Int>) -> Graph<B> {
        let embedded_nodes = self.embed_nodes(graph.nodes);
        Graph::new(embedded_nodes, graph.edges)
    }

    pub fn compress_embedded_graph(&self, embedded_graph: Graph<B>) -> Tensor<B, 1> {
        let mut extracted_graph = embedded_graph;
        for (aggregate_layers, pooling_layer, norm_layer) in &self.compression_layers {
            for aggregate_layer in aggregate_layers {
                extracted_graph = aggregate_layer.forward(extracted_graph);
            }
            extracted_graph = pooling_layer.forward(extracted_graph);
            extracted_graph.nodes = norm_layer.forward(extracted_graph.nodes);
        }
        extracted_graph.nodes.max_dim(0).squeeze_dim(0)
    }

    pub fn predict_embedded_graphs(
        &self,
        embedded_graph_1: Graph<B>,
        embedded_graph_2: Graph<B>,
    ) -> Tensor<B, 1> {
        let compressed_graph_1 = self.compress_embedded_graph(embedded_graph_1);
        let compressed_graph_2 = self.compress_embedded_graph(embedded_graph_2);
        let commons = compressed_graph_1.clone() * compressed_graph_2.clone(); 
        let differences = (compressed_graph_1 - compressed_graph_2).abs();
        let distance_gate =sigmoid(self.common_gate.forward(commons));
        let gated_distance = distance_gate * differences;
        let weighted_distance = self.distance_weights.forward(self.dropout.forward(gated_distance));
        let similarity = Tensor::exp(-relu(weighted_distance));

        similarity
    }

    pub fn forward(&self, graph_1: Graph<B, Int>, graph_2: Graph<B, Int>) -> Tensor<B, 1> {
        self.predict_embedded_graphs(self.embed_graph(graph_1), self.embed_graph(graph_2))
    }
}
