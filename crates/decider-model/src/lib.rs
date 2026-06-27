pub mod data;
pub mod layers;
mod model;

use burn::tensor::backend::Backend;
use burn_store::{ModuleSnapshot, ModuleStore};
pub use model::*;

pub fn model_config() -> PlagiarismDeciderConfig {
    PlagiarismDeciderConfig::new(u16::MAX as usize + 1, 16, 128, 0.1, 32)
}

pub fn load_model<B: Backend, P: ModuleStore>(
    device: &B::Device,
    store: &mut P,
) -> PlagiarismDecider<B> {
    let mut model = init_model::<B>(device);
    model.load_from(store).unwrap();
    model
}

pub fn init_model<B: Backend>(device: &B::Device) -> PlagiarismDecider<B> {
    model_config().init::<B>(device)
}
