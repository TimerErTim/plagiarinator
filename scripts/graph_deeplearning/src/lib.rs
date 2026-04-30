use crate::nn::{PlagiarismDeciderConfig, PlagiarismDeciderLayerConfig};

pub mod loading;
pub mod model;
pub mod nn;

pub fn get_model_config() -> PlagiarismDeciderConfig {
    PlagiarismDeciderConfig::new(
        u16::MAX as usize + 1,
        64,
        128,
        0.1,
        vec![
            PlagiarismDeciderLayerConfig::new(128, 1024),
            PlagiarismDeciderLayerConfig::new(392, 128),
            PlagiarismDeciderLayerConfig::new(1024, 16),
            PlagiarismDeciderLayerConfig::new(4096, 1),
        ],
    )
}