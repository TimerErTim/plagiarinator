use std::num::{NonZero, NonZeroUsize};

use burn::{Tensor, config::Config, module::{Module, Param, Parameter}, nn::{Initializer, Linear, LinearConfig}, prelude::Backend, tensor::activation::{leaky_relu, log_softmax, softmax}};

use crate::model::{Graph, graph_convolution};

mod decider;

pub use decider::*;


#[derive(Config, Debug)]
pub struct GraphConvolutionConfig {
    #[config(default = true)]
    with_bias: bool,
    input_features: usize,
    output_features: usize,
    /// The type of function used to initialize neural network parameters
    #[config(
        default = "Initializer::KaimingUniform{gain:1.0/f64::sqrt(3.0), fan_out_only:false}"
    )]
    pub initializer: Initializer,
}

#[derive(Module, Debug)]
pub struct GraphConvolution<B: Backend> {
    weights: Linear<B>,
}

impl GraphConvolutionConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GraphConvolution<B> {
        let config = LinearConfig::new(self.input_features, self.output_features).with_bias(self.with_bias).with_initializer(self.initializer.clone());
        let weights = config.init(device);
        GraphConvolution { weights }
    }
}

impl<B: Backend> GraphConvolution<B> {
    pub fn forward(&self, input: Graph<B>) -> Graph<B> {
        graph_convolution(input, self.weights.weight.val(), self.weights.bias.as_ref().map(|bias| bias.val()))
    }
}

#[derive(Config, Debug)]
pub struct GraphDiffPoolConfig {
    input_features: usize,
    output_features: usize,
    num_clusters: usize,
    #[config(default = true)]
    with_bias: bool,
    #[config(default = "Initializer::KaimingUniform{gain:1.0/f64::sqrt(3.0), fan_out_only:false}")]
    initializer: Initializer,
}

#[derive(Module, Debug)]
pub struct GraphDiffPool<B: Backend> {
    pooling_layer: GraphConvolution<B>,
    embed_layer: GraphConvolution<B>,
}

impl GraphDiffPoolConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GraphDiffPool<B> {
        GraphDiffPool {
            pooling_layer: GraphConvolutionConfig::new(self.input_features, self.num_clusters).with_with_bias(self.with_bias).with_initializer(self.initializer.clone()).init(device),
            embed_layer: GraphConvolutionConfig::new(self.input_features, self.output_features).with_with_bias(self.with_bias).with_initializer(self.initializer.clone()).init(device),
        }
    }
}

impl<B: Backend> GraphDiffPool<B> {
    pub fn forward(&self, input: Graph<B>) -> Graph<B> {        
        let soft_assignments = log_softmax(self.pooling_layer.forward(input.clone()).nodes, 0);
        let node_embeddings = leaky_relu(self.embed_layer.forward(input.clone()).nodes, 0.1);
        let super_nodes = soft_assignments.clone().transpose().matmul(node_embeddings);
        let super_edges = soft_assignments.clone().transpose().matmul(input.edges).matmul(soft_assignments);
        Graph::new(super_nodes, super_edges)
    }
}