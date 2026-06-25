use burn::{backend::Autodiff, prelude::Backend};
use burn_store::ModuleSnapshot;
use decider_model::{PlagiarismDecider, load_model};

pub mod validation;
pub mod loading;
pub mod logging;

pub fn load_trained_model<B: Backend>(device: &B::Device) -> PlagiarismDecider<B> {
    let model_weights_path = std::env::var("MODEL_WEIGHTS_PATH").unwrap();
    let mut store = burn::store::BurnpackStore::from_file(model_weights_path);
    load_model(device, &mut store)
}
