use std::num::{NonZero, NonZeroUsize};

use burn::{Tensor, config::Config, module::{Module, Param, Parameter}, nn::{Initializer, Linear, LinearConfig}, prelude::Backend};

use crate::model::{Graph, edge_pseudonodes};


#[derive(Config, Debug)]
pub struct GraphCompressionLayerConfig {
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
pub struct GraphCompressionLayer<B: Backend> {
    edge_collapsing: Linear<B>,
    orphan_distr: Linear<B>,
}

impl GraphCompressionLayerConfig {
    pub fn init<B: Backend>(&self, device: &B::Device) -> GraphCompressionLayer<B> {
        let config = LinearConfig::new(self.input_features, self.output_features).with_bias(self.with_bias).with_initializer(self.initializer.clone());
        let edge_collapsing = config.init(device);
        let orphan_distr = config.init(device);
        GraphCompressionLayer { edge_collapsing, orphan_distr }
    }
}

impl<B: Backend> GraphCompressionLayer<B> {
    pub fn forward(&self, input: Graph<B>) -> Graph<B> where B::IntElem: Into<i64> {
        edge_pseudonodes(input, self.edge_collapsing.weight.val(), self.edge_collapsing.bias.as_ref().map(|bias| bias.val()), self.orphan_distr.weight.val(), self.orphan_distr.bias.as_ref().map(|bias| bias.val()))
    }
}
