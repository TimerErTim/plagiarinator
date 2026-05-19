pub mod data;
pub mod layers;
mod model;

use burn_store::{ModuleSnapshot, ModuleStore};
pub use model::*;
use burn::tensor::backend::Backend;

pub fn model_config() -> PlagiarismDeciderConfig {
    PlagiarismDeciderConfig::new(
        u16::MAX as usize + 1,
        32,
        128,
        0.1,
        vec![
            PlagiarismDeciderLayerConfig::new(64, 1024).with_pre_aggregations(0),
            PlagiarismDeciderLayerConfig::new(128, 256).with_pre_aggregations(0),
            PlagiarismDeciderLayerConfig::new(256, 64).with_pre_aggregations(0),
            PlagiarismDeciderLayerConfig::new(512, 16).with_pre_aggregations(0),
            PlagiarismDeciderLayerConfig::new(2048, 1).with_pre_aggregations(0),
        ],
    )
}

pub fn load_model<B: Backend, P: ModuleStore>(device: &B::Device, store: &mut P) -> PlagiarismDecider<B> {
    let mut model = init_model::<B>(device);
    model.load_from(store).unwrap();
    model
}

pub fn init_model<B: Backend>(device: &B::Device) -> PlagiarismDecider<B> {
    model_config().init::<B>(device)
}
