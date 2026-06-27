use burn::prelude::Backend;
use decider_model::{load_model, PlagiarismDecider};

pub mod loading;
pub mod logging;
pub mod validation;

pub fn load_trained_model<B: Backend>(device: &B::Device) -> PlagiarismDecider<B> {
    let model_weights_path = std::env::var("MODEL_WEIGHTS_PATH").unwrap();
    let mut store = burn::store::BurnpackStore::from_file(model_weights_path);
    load_model(device, &mut store)
}
