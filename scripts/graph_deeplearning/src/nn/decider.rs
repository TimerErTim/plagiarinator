use burn::{Tensor, config::Config, module::Module, nn::{Dropout, DropoutConfig, Embedding, EmbeddingConfig, LayerNorm, LayerNormConfig, Linear, LinearConfig}, prelude::Backend, tensor::{Int, activation::{leaky_relu, sigmoid, silu}}};

use crate::{model::Graph, nn::{GraphDiffPool, GraphDiffPoolConfig}};



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
}

impl PlagiarismDeciderConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> PlagiarismDecider<B> {
        let mut compression_layers: Vec<(GraphDiffPoolConfig, LayerNormConfig)> = Vec::with_capacity(self.layers.len());
        for layer_config in &self.layers {
            let prev_layer_features = if let Some(prev_layer) = compression_layers.last() {
                prev_layer.0.output_features
            } else {
                self.embedding_size
            };
            compression_layers.push((GraphDiffPoolConfig::new(prev_layer_features, layer_config.output_features, layer_config.num_clusters), LayerNormConfig::new(layer_config.output_features)));
        }
        let last_layer_features = if let Some(last_layer) = compression_layers.last() {
            last_layer.0.output_features
        } else {
            self.embedding_size
        };

        PlagiarismDecider {
            embedding: EmbeddingConfig::new(self.num_classes, self.embedding_size).init(device),
            compression_layers: compression_layers.into_iter().map(|(pool_config, norm_config)| (pool_config.init(device), norm_config.init(device))).collect(),
            dropout: DropoutConfig::new(self.dropout_rate).init(),
            comparator: LinearConfig::new(last_layer_features * 2, self.comparator_size).init(device),
            output: LinearConfig::new(self.comparator_size, 1).init(device),
        }
    }
}

#[derive(Module, Debug)]
pub struct PlagiarismDecider<B: Backend> {
    embedding: Embedding<B>,
    compression_layers: Vec<(GraphDiffPool<B>, LayerNorm<B>)>,
    dropout: Dropout,
    comparator: Linear<B>,
    output: Linear<B>,
}

impl<B: Backend> PlagiarismDecider<B> {
    pub fn forward(&self, graph_1: Graph<B, Int>, graph_2: Graph<B, Int>) -> Tensor<B, 1> {
        let compress_graph = |graph: Graph<B, Int>| {
            let embedded_nodes = self.embedding.forward(graph.nodes);
            let embedded_graph = Graph::new(embedded_nodes.squeeze_dim(0), graph.edges);
            let mut extracted_graph = embedded_graph;
            for (pooling_layer, norm_layer) in &self.compression_layers {
                extracted_graph = pooling_layer.forward(extracted_graph);
                extracted_graph.nodes = silu(extracted_graph.nodes);
                extracted_graph.nodes = norm_layer.forward(extracted_graph.nodes);
            }
            let nodes_features = extracted_graph.nodes;
            nodes_features.max_dim(0).squeeze_dim::<1>(0)
        };
        let compressed_graph_1 = compress_graph(graph_1);
        let compressed_graph_2 = compress_graph(graph_2);
        let comparator_input_left = Tensor::cat(vec![compressed_graph_1.clone(), compressed_graph_2.clone()], 0);
        let comparator_input_right = Tensor::cat(vec![compressed_graph_2, compressed_graph_1], 0);
        let comparator_input = Tensor::stack::<2>(vec![comparator_input_left, comparator_input_right], 0);
        let comparator_input = self.dropout.forward(comparator_input);

        let comparator_output = silu(self.comparator.forward(comparator_input));
        let output = self.output.forward(comparator_output);
        sigmoid(output).mean()
    }
}