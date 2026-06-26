use burn::{
    Tensor,
    config::Config,
    module::Module,
    nn::{Dropout, DropoutConfig, Embedding, EmbeddingConfig, LayerNorm, LayerNormConfig, Linear, LinearConfig},
    prelude::Backend,
    tensor::{Int, activation::{sigmoid, silu}},
};

use crate::{
    data::Graph,
    layers::{GINConv, GINConvConfig, GraphCompress, GraphCompressConfig},
};

#[derive(Config, Debug)]
pub struct PlagiarismDeciderConfig {
    num_classes: usize,
    embedding_size: usize,
    comparator_size: usize,
    dropout_rate: f64,
    // The number of output features for each layer
    layers: usize,
}

impl PlagiarismDeciderConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> PlagiarismDecider<B> {

        PlagiarismDecider {
            embedding: EmbeddingConfig::new(self.num_classes, self.embedding_size).init(device),
            graph_compression: GINConvConfig::new(self.embedding_size, self.layers).init(device),
            diff_norm: LayerNormConfig::new(self.embedding_size * (self.layers + 1)).init(device),
            dropout: DropoutConfig::new(self.dropout_rate).init(),
            comparator: LinearConfig::new(self.embedding_size * (self.layers + 1), self.comparator_size).init(device),
            decider: LinearConfig::new(self.comparator_size, 1).init(device),
        }
    }
}

#[derive(Module, Debug)]
pub struct PlagiarismDecider<B: Backend> {
    pub embedding: Embedding<B>,
    graph_compression: GINConv<B>,
    diff_norm: LayerNorm<B>,
    dropout: Dropout,
    comparator: Linear<B>,
    decider: Linear<B>,
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
        let compressed_graph = self.graph_compression.forward(embedded_graph);
        compressed_graph.flatten(0, 1)
    }

    pub fn predict_embedded_graphs(
        &self,
        embedded_graph_1: Graph<B>,
        embedded_graph_2: Graph<B>,
    ) -> Tensor<B, 1> {
        let compressed_graph_1 = self.compress_embedded_graph(embedded_graph_1);
        let compressed_graph_2 = self.compress_embedded_graph(embedded_graph_2);
        let differences = Tensor::abs(compressed_graph_1 - compressed_graph_2);
        let normalized_differences = self.dropout.forward(self.diff_norm.forward(differences));
        let compared = silu(self.comparator.forward(normalized_differences));
        let decision = self.decider.forward(compared);
        sigmoid(decision).mean()
    }

    pub fn forward(&self, graph_1: Graph<B, Int>, graph_2: Graph<B, Int>) -> Tensor<B, 1> {
        self.predict_embedded_graphs(self.embed_graph(graph_1), self.embed_graph(graph_2))
    }
}
