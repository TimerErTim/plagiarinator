use std::num::{NonZero, NonZeroUsize};

use burn::{Tensor, config::Config, module::{Module, Param, Parameter}, nn::Initializer, prelude::Backend};

use crate::model::{Graph, edge_pseudonodes};


#[derive(Config, Debug)]
pub struct GraphCompressionLayerConfig {
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
pub struct GraphCompressionLayer<B: Backend> {
    weights: Param<Tensor<B, 2>>,
    bias: Option<Param<Tensor<B, 1>>>,
}

impl GraphCompressionLayerConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GraphCompressionLayer<B> {
        let weights = self.initializer.init_with([self.input_features, self.output_features], Some(self.input_features), Some(self.output_features), device);
        let bias = if self.with_bias { Some(self.initializer.init_with([self.output_features], Some(self.input_features), Some(self.output_features), device)) } else { None };
        GraphCompressionLayer { weights, bias }
    }
}

impl<B: Backend> GraphCompressionLayer<B> {
    pub fn forward(&self, device: &B::Device, input: Graph<B>) -> Graph<B> where B::IntElem: Into<i64> {
        edge_pseudonodes(device, input, self.weights.val(), self.bias.as_ref().map(|bias| bias.val()).unwrap_or_else(|| Tensor::zeros([self.weights.val().shape()[1]], device)), NonZeroUsize::new(1).unwrap(), NonZeroUsize::new(1).unwrap())
    }
}
