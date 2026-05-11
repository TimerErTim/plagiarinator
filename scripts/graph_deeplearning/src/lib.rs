use crate::nn::{PlagiarismDeciderConfig, PlagiarismDeciderLayerConfig};

pub mod loading;
pub mod model;
pub mod nn;

pub fn get_model_config() -> PlagiarismDeciderConfig {
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