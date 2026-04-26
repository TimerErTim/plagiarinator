use burn::{Tensor, config::Config, module::Module, nn::{Dropout, DropoutConfig, Embedding, EmbeddingConfig, Linear, LinearConfig}, prelude::Backend, tensor::{Int, activation::{leaky_relu, sigmoid, silu}}};

use crate::{model::Graph, nn::{GraphCompressionLayer, GraphCompressionLayerConfig}};


#[derive(Config, Debug)]
pub struct PlagiarismDeciderConfig {
    num_classes: usize,
    embedding_size: usize,
    comparator_size: usize,
    dropout_rate: f64,
    // The number of output features for each layer
    layers: Vec<usize>,
}

impl PlagiarismDeciderConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> PlagiarismDecider<B> {
        let mut compression_layers: Vec<GraphCompressionLayerConfig> = Vec::with_capacity(self.layers.len());
        for layer_features in &self.layers {
            let prev_layer_features = if let Some(prev_layer) = compression_layers.last() {
                prev_layer.output_features
            } else {
                self.embedding_size
            };
            compression_layers.push(GraphCompressionLayerConfig::new(prev_layer_features, *layer_features));
        }
        let last_layer_features = if let Some(last_layer) = compression_layers.last() {
            last_layer.output_features
        } else {
            self.embedding_size
        };

        PlagiarismDecider {
            embedding: EmbeddingConfig::new(self.num_classes, self.embedding_size).init(device),
            compression_layers: compression_layers.into_iter().map(|config| config.init(device)).collect(),
            dropout: DropoutConfig::new(self.dropout_rate).init(),
            comparator: LinearConfig::new(last_layer_features * 2, self.comparator_size).init(device),
            output: LinearConfig::new(self.comparator_size, 1).init(device),
        }
    }
}

#[derive(Module, Debug)]
pub struct PlagiarismDecider<B: Backend> {
    embedding: Embedding<B>,
    compression_layers: Vec<GraphCompressionLayer<B>>,
    dropout: Dropout,
    comparator: Linear<B>,
    output: Linear<B>,
}

impl<B: Backend> PlagiarismDecider<B> {
    pub fn forward(&self, graph_1: Graph<B, Int>, graph_2: Graph<B, Int>) -> Tensor<B, 1> where B::IntElem: Into<i64> {
        let compress_graph = |graph: Graph<B, Int>| {
            let embedded_nodes = self.embedding.forward(graph.nodes.swap_dims(0, 1));
            let embedded_graph = if let Some(edges) = graph.edges {
                Graph::new(embedded_nodes.squeeze(), edges)
            } else {
                Graph::unconnected(embedded_nodes.squeeze())
            };
            let mut extracted_graph = embedded_graph;
            for compression_layer in &self.compression_layers {
                extracted_graph = compression_layer.forward(extracted_graph);
                extracted_graph.nodes = leaky_relu(extracted_graph.nodes, 0.1);
            }
            let nodes_features = extracted_graph.nodes;
            let dropped_nodes_features = self.dropout.forward(nodes_features);
            dropped_nodes_features.max_dim(0).squeeze::<1>()
        };
        let compressed_graph_1 = compress_graph(graph_1);
        let compressed_graph_2 = compress_graph(graph_2);
        let comparator_input = Tensor::cat(vec![compressed_graph_1, compressed_graph_2], 0);

        let comparator_output = silu(self.comparator.forward(comparator_input));
        let output = self.output.forward(comparator_output);
        sigmoid(output)
    }
}