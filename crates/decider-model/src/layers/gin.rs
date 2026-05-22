// Graph Isomorphism Network

use burn::{
    Tensor, config::Config, module::Module, nn::{BatchNorm, Dropout, DropoutConfig, Embedding, EmbeddingConfig, LayerNorm, LayerNormConfig, Linear, LinearConfig}, prelude::Backend, tensor::{Int, activation::{relu, sigmoid}}
};

use crate::data::Graph;

/// A single MLP layer in the Graph Isomorphism Network
#[derive(Config, Debug)]
pub struct GINMLPConfig {
    pub features_per_node: usize,
    #[config(default = 1)]
    pub hidden_layers: usize,
    #[config(default = 0.0)]
    pub epsilon: f32,
    #[config(default = false)]
    pub layer_norm: bool,
}

impl GINMLPConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GINMLP<B> {
        GINMLP {
            // Output layer let's not forget
            hidden_layers: (0..self.hidden_layers).map(|_| LinearConfig::new(self.features_per_node, self.features_per_node).init(device)).collect(),
            output_layer: LinearConfig::new(self.features_per_node, self.features_per_node).init(device),
            layer_norm: if self.layer_norm { Some(LayerNormConfig::new(self.features_per_node).init(device)) } else { None },
            epsilon: self.epsilon,
        }
    }
}

#[derive(Module, Debug)]
pub struct GINMLP<B: Backend> {
    pub hidden_layers: Vec<Linear<B>>,
    pub output_layer: Linear<B>,
    pub layer_norm: Option<LayerNorm<B>>,
    pub epsilon: f32,
}

impl<B: Backend> GINMLP<B> {
    pub fn forward(&self, graph: Graph<B>) -> Graph<B> {
        let mlp_input = (1.0 + self.epsilon) * graph.nodes.clone() + graph.edges.clone().matmul(graph.nodes);

        // Pass through MLP
        let output = self.hidden_layers.iter().fold(mlp_input, |mut x, layer| {
            x = layer.forward(x);
            x = relu(x);
            if let Some(layer_norm) = &self.layer_norm {
                x = layer_norm.forward(x);
            }
            x
        });
        let new_nodes = self.output_layer.forward(output);
        Graph::new(new_nodes, graph.edges)
    }
}


#[derive(Config, Debug)]
pub struct GINConvConfig {
    pub features_per_node: usize,
    pub layers: usize,
    #[config(default = 0.0)]
    pub epsilon: f32,
    #[config(default = false)]
    pub layer_norm: bool,
}

impl GINConvConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GINConv<B> {
        GINConv {
            layers: (0..self.layers).map(|_| GINMLPConfig::new(self.features_per_node).with_epsilon(self.epsilon).with_layer_norm(true).init(device)).collect(),
            layer_norm: if self.layer_norm { Some(LayerNormConfig::new(self.features_per_node).init(device)) } else { None },
        }
    }
}

#[derive(Module, Debug)]
pub struct GINConv<B: Backend> {
    layers: Vec<GINMLP<B>>,
    layer_norm: Option<LayerNorm<B>>,
}

impl<B: Backend> GINConv<B> {
    pub fn forward(&self, graph: Graph<B>) -> Tensor<B, 2> {
        let layer_features = std::iter::once(graph.nodes.clone().sum_dim(0)).chain(self.layers.iter().scan(graph, |prev_graph, mlp_layer| {
            let next_graph = mlp_layer.forward(prev_graph.clone());
            let next_nodes_aggregated = next_graph.nodes.clone().sum_dim(0);
            *prev_graph = next_graph;
            Some(next_nodes_aggregated)
        }));
        let mut concatenated_features = Tensor::cat(layer_features.collect::<Vec<_>>(), 0);
        if let Some(layer_norm) = &self.layer_norm {
            concatenated_features = layer_norm.forward(concatenated_features);
        }
        concatenated_features
    }
}