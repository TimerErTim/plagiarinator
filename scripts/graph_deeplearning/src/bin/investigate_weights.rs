use burn::prelude::Backend;
use burn_store::ModuleStore;
use decider_model::{load_model, PlagiarismDecider};

pub fn main() {
    let model_weights_path = std::env::var("MODEL_WEIGHTS_PATH").unwrap();
    eprintln!("Loading model from {}", model_weights_path);
    let mut store = burn::store::BurnpackStore::from_file(model_weights_path);
    dbg!(store.keys().unwrap());
}
