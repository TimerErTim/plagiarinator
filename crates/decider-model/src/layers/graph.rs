use burn::{
    config::Config,
    module::Module,
    nn::{Initializer, LayerNorm, LayerNormConfig, SwiGlu, SwiGluConfig},
    prelude::Backend,
    tensor::{Float, activation::log_softmax},
};

use crate::data::Graph;

pub fn graph_convolution<B: Backend>(graph: Graph<B, Float>, swiglu: SwiGlu<B>) -> Graph<B> {
    let normalized_adjacency_matrix = graph.normalized_adjacency_matrix();
    // Aggregate features of nodes pointing to us, transpose would result in features we point to
    // (but transposing would be required for all downstream operations)
    let neighbor_features = normalized_adjacency_matrix.matmul(graph.nodes);
    let neighbor_features = swiglu.forward(neighbor_features);
    Graph {
        nodes: neighbor_features,
        edges: graph.edges,
    }
}

#[derive(Config, Debug)]
pub struct GraphConvConfig {
    input_features: usize,
    output_features: usize,
    #[config(default = true)]
    bias: bool,
    /// The type of function used to initialize neural network parameters
    #[config(default = "Initializer::KaimingUniform{gain:1.0/f64::sqrt(3.0), fan_out_only:false}")]
    pub initializer: Initializer,
    #[config(default = false)]
    pub normalization: bool,
}

#[derive(Module, Debug)]
pub struct GraphConv<B: Backend> {
    swiglu: SwiGlu<B>,
    normalization: Option<LayerNorm<B>>,
}

impl GraphConvConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GraphConv<B> {
        GraphConv {
            swiglu: SwiGluConfig::new(self.input_features, self.output_features)
                .with_bias(self.bias)
                .with_initializer(self.initializer.clone())
                .init(device),
            normalization: if self.normalization {
                Some(LayerNormConfig::new(self.output_features).init(device))
            } else {
                None
            },
        }
    }
}

impl<B: Backend> GraphConv<B> {
    pub fn forward(&self, input: Graph<B>) -> Graph<B> {
        let mut output = graph_convolution(input, self.swiglu.clone());
        if let Some(normalization) = &self.normalization {
            output.nodes = normalization.forward(output.nodes);
        }
        output
    }
}

// Unused for now, may become relevant later if separate aggregation cycles per soft assignment and super node embeddings are needed
// #[derive(Config, Debug)]
// pub struct GraphConvSeriesConfig {
//     input_features: usize,
//     output_features: usize,
//     #[config(default = true)]
//     bias: bool,
//     #[config(default = 0)]
//     post_repetition: usize,
// }

#[derive(Config, Debug)]
pub struct GraphDiffPoolConfig {
    input_features: usize,
    output_features: usize,
    num_clusters: usize,
    #[config(default = true)]
    bias: bool,
    #[config(default = "Initializer::KaimingUniform{gain:1.0/f64::sqrt(3.0), fan_out_only:false}")]
    initializer: Initializer,
    #[config(default = false)]
    normalization: bool,
}

#[derive(Module, Debug)]
pub struct GraphDiffPool<B: Backend> {
    assignments_layer: GraphConv<B>,
    embed_layer: GraphConv<B>,
    normalization: Option<LayerNorm<B>>,
}

impl GraphDiffPoolConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GraphDiffPool<B> {
        GraphDiffPool {
            assignments_layer: GraphConvConfig::new(self.input_features, self.num_clusters)
                .with_bias(self.bias)
                .with_initializer(self.initializer.clone())
                .with_normalization(false)
                .init(device),
            embed_layer: GraphConvConfig::new(self.input_features, self.output_features)
                .with_bias(self.bias)
                .with_initializer(self.initializer.clone())
                .with_normalization(false)
                .init(device),
            normalization: if self.normalization {
                Some(LayerNormConfig::new(self.output_features).init(device))
            } else {
                None
            },
        }
    }
}

impl<B: Backend> GraphDiffPool<B> {
    pub fn forward(&self, input: Graph<B>) -> Graph<B> {
        let soft_assignments = log_softmax(self.assignments_layer.forward(input.clone()).nodes, 0);
        let node_embeddings = self.embed_layer.forward(input.clone()).nodes;
        let mut super_nodes = soft_assignments.clone().transpose().matmul(node_embeddings);
        let super_edges = soft_assignments
            .clone()
            .transpose()
            .matmul(input.edges)
            .matmul(soft_assignments);
        if let Some(normalization) = &self.normalization {
            super_nodes = normalization.forward(super_nodes);
        }
        Graph::new(super_nodes, super_edges)
    }
}

#[derive(Config, Debug)]
pub struct GraphCompressConfig {
    pub input_features: usize,
    pub output_features: usize,
    pub super_nodes: usize,
    /// The number of aggregation layers before the pooling layer
    #[config(default = 0)]
    pub pre_aggregations: usize,
    /// Output normalization of super nodes
    #[config(default = false)]
    pub normalization: bool,
}

#[derive(Module, Debug)]
pub struct GraphCompress<B: Backend> {
    pre_aggregations: Vec<GraphConv<B>>,
    pooling_layer: GraphDiffPool<B>,
}

impl GraphCompressConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GraphCompress<B> {
        GraphCompress {
            pre_aggregations: (0..self.pre_aggregations)
                .map(|_| {
                    GraphConvConfig::new(self.input_features, self.input_features)
                        .with_normalization(true)
                        .with_bias(true)
                        .init(device)
                })
                .collect(),
            pooling_layer: GraphDiffPoolConfig::new(
                self.input_features,
                self.output_features,
                self.super_nodes,
            )
            .with_normalization(self.normalization)
            .init(device),
        }
    }
}

impl<B: Backend> GraphCompress<B> {
    pub fn forward(&self, input: Graph<B>) -> Graph<B> {
        let mut output = input;
        for aggregation in &self.pre_aggregations {
            // Normalization of nodes is performed by the aggregation layer
            output = aggregation.forward(output);
        }
        // Normalization of super nodes is performed by the pooling layer
        output = self.pooling_layer.forward(output);
        output
    }
}
