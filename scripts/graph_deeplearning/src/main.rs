use std::{fs::File, io::BufReader, num::{NonZeroU16, NonZeroUsize}};

use burn::{
    nn::norm::NormalizationRecordItem,
    prelude::Backend,
    tensor::{ops::IntTensorOps, Float, Int},
    Tensor,
};
use data_loading::dataset_loader::load_dataset;

use graph_deeplearning::{layers::{GraphCompressionLayer, GraphCompressionLayerConfig}, loading::{IterUtilsExt, PrefetchIterator, make_trainset_loader, parse_cpp_to_tree}, model::Graph};
use rand::SeedableRng;

pub fn main() {
    type InfBackend = burn::backend::NdArray;
    let device = burn::backend::ndarray::NdArrayDevice::Cpu;
    InfBackend::seed(&device, 32);
    let mut rng = rand::rngs::SmallRng::seed_from_u64(32);

    let dataset = load_dataset("datasets/c_cpp_plagiarism").unwrap();

    let (cpp_train_dataset, cpp_test_dataset) = dataset.cpp_dataset.split_dataset(0.8, &mut rng);
    let train_loader = make_trainset_loader::<InfBackend>(cpp_train_dataset.plagiarized_pairs, cpp_train_dataset.authentic_pairs, 32, rng.clone(), &device);

    let train_loader = PrefetchIterator::new(train_loader, 5);

    for item in train_loader {
        println!("item: {item:?}");
    }
}
