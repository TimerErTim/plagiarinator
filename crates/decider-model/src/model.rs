use burn::{
    Tensor, config::Config, module::Module, nn::{Dropout, DropoutConfig, Embedding, EmbeddingConfig, LayerNorm, LayerNormConfig, Linear, LinearConfig, SwiGlu, SwiGluConfig}, prelude::Backend, tensor::{Int, activation::{gelu, tanh}, linalg::cosine_similarity},
};

use crate::{
    data::Graph,
    layers::{GINConv, GINConvConfig},
};

#[derive(Config, Debug)]
pub struct PlagiarismDeciderConfig {
    num_classes: usize,
    embedding_size: usize,
    representation_size: usize,
    dropout_rate: f64,
    // The number of output features for each layer
    layers: usize,
}

impl PlagiarismDeciderConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> PlagiarismDecider<B> {
        let gin_flat_size = self.embedding_size * (self.layers + 1);
        
        PlagiarismDecider {
            embedding: EmbeddingConfig::new(self.num_classes, self.embedding_size).init(device),
            graph_compression: GINConvConfig::new(self.embedding_size, self.layers).init(device),
            graph_compression_norm: LayerNormConfig::new(gin_flat_size).init(device),
            graph_representation_hidden: SwiGluConfig::new(gin_flat_size, gin_flat_size).init(device),
            graph_representation: LinearConfig::new(gin_flat_size, self.representation_size).init(device),
            dropout: DropoutConfig::new(self.dropout_rate).init(),
        }
    }
}

#[derive(Module, Debug)]
pub struct PlagiarismDecider<B: Backend> {
    pub embedding: Embedding<B>,
    graph_compression: GINConv<B>,
    graph_compression_norm: LayerNorm<B>,
    graph_representation_hidden: SwiGlu<B>,
    graph_representation: Linear<B>,
    dropout: Dropout,
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
        let compressed_graph = compressed_graph.flatten(0, 1);
        let compressed_graph = self.graph_compression_norm.forward(compressed_graph);
        let compressed_graph = self.graph_representation_hidden.forward(compressed_graph);
        let compressed_graph = self.dropout.forward(compressed_graph);
        tanh(self.graph_representation.forward(compressed_graph))
    }

    pub fn predict_embedded_graphs(
        &self,
        embedded_graph_1: Graph<B>,
        embedded_graph_2: Graph<B>,
    ) -> Tensor<B, 1> {
        self.similarity_probability(
            self.compress_embedded_graph(embedded_graph_1),
            self.compress_embedded_graph(embedded_graph_2),
        )
    }

    pub fn similarity_probability(&self, compressed_graph_1: Tensor<B, 1>, compressed_graph_2: Tensor<B, 1>) -> Tensor<B, 1> {
        let similarity = cosine_similarity(compressed_graph_1, compressed_graph_2, 0, None);
        let probability = similarity / 2.001 + 0.5; // Range [>0, <1]
        probability
    }

    pub fn forward(&self, graph_1: Graph<B, Int>, graph_2: Graph<B, Int>) -> Tensor<B, 1> {
        self.predict_embedded_graphs(
            self.embed_graph(graph_1),
            self.embed_graph(graph_2)
        )
    }
}
