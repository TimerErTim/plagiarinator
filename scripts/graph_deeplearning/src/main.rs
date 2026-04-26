use burn::{
    Tensor, backend::Autodiff, grad_clipping::GradientClippingConfig, module::{AutodiffModule, Module}, nn::loss::BinaryCrossEntropyLossConfig, optim::{AdamConfig, GradientsParams, Optimizer}, prelude::Backend
};
#[cfg(feature = "cuda")]
use burn::backend::cuda::CudaDevice;
#[cfg(not(feature = "cuda"))]
use burn::backend::ndarray::NdArrayDevice;
use data_loading::dataset_loader::load_dataset;

use graph_deeplearning::{loading::{PrefetchIterator, chunked_iter, make_testset, make_trainset_loader}, model::PlagiarismTrainItem, nn::{PlagiarismDecider, PlagiarismDeciderConfig}};
use rand::SeedableRng;


pub fn main() {
    #[cfg(feature = "cuda")]
    type InfBackend = burn::backend::Cuda;
    #[cfg(not(feature = "cuda"))]
    type InfBackend = burn::backend::NdArray;
    type AdBackend = Autodiff<InfBackend>;
    #[cfg(feature = "cuda")]
    let device = CudaDevice::default();
    #[cfg(not(feature = "cuda"))]
    let device = NdArrayDevice::Cpu;
    #[cfg(feature = "cuda")]
    println!("graph_deeplearning: using Burn CUDA backend, device {device:?}");
    #[cfg(not(feature = "cuda"))]
    println!("graph_deeplearning: using Burn NdArray backend (CPU)");
    AdBackend::seed(&device, 32);
    let batch_size = 32;
    let mut rng = rand::rngs::SmallRng::seed_from_u64(32);
    let validation_interval = 10;
    let learning_rate = 0.001;

    let dataset = load_dataset("datasets/c_cpp_plagiarism").unwrap();

    let (cpp_train_dataset, cpp_test_dataset) = dataset.cpp_dataset.split_dataset(0.8, &mut rng);
    let cpp_test_items = make_testset::<InfBackend>(cpp_test_dataset.plagiarized_pairs, cpp_test_dataset.authentic_pairs, &device);
    let num_train_items = cpp_train_dataset.plagiarized_pairs.len() + cpp_train_dataset.authentic_pairs.len();
    println!("num_train_items: {num_train_items}");
    let train_loader = make_trainset_loader::<AdBackend>(cpp_train_dataset.plagiarized_pairs, cpp_train_dataset.authentic_pairs, rng.clone(), &device);

    let train_loader = PrefetchIterator::new(train_loader, batch_size * 2);
    let batched_loader = chunked_iter(train_loader, batch_size);

    // init model
    let model_config = PlagiarismDeciderConfig::new(
        u16::MAX as usize,
        64,
        64,
        0.1,
        vec![96, 128, 192, 256],
    );
    let mut model = model_config.init::<AdBackend>(&device);
    println!("model: {model}");

    // init optimizer and loss function
    let mut optimizer = AdamConfig::new()
        .with_grad_clipping(Some(GradientClippingConfig::Value(1.0)))
        .init();
    let loss_fn = BinaryCrossEntropyLossConfig::new().init(&device);

    for (idx, batch) in batched_loader.enumerate() {
        let losses = batch.into_iter().map(|item| {
            let prediction = model.forward(item.graph_1.clone(), item.graph_2.clone());
            let loss = loss_fn.forward(prediction.clone(), Tensor::from_ints([if item.label { 1 } else { 0 }], &device));
            loss
        }).collect();
        let average_loss = Tensor::cat(losses, 0).mean();
        let mut grads = average_loss.backward();
        let param_grads = GradientsParams::from_module(&mut grads, &model);
        model = optimizer.step(learning_rate, model, param_grads);

        if idx % validation_interval == 0 {
            let validation_output = validate(model.valid(), &cpp_test_items);
            println!("step {idx}, \tvalidation_output: {validation_output:?}");
        }
    }
}

#[derive(Debug)]
pub struct ValidationOutput {
    pub average_loss: f64,
    pub true_positive: usize,
    pub true_negative: usize,
    pub false_positive: usize,
    pub false_negative: usize,
}

pub fn validate<B: Backend>(model: PlagiarismDecider<B>, test_dataset: &Vec<PlagiarismTrainItem<B>>) -> ValidationOutput
where
    B::IntElem: Into<i64>,
    B::FloatElem: Into<f64>,
{
    let mut average_loss = 0.0;
    let mut true_positive = 0;
    let mut true_negative = 0;
    let mut false_positive = 0;
    let mut false_negative = 0;
    let device = model.devices().first().unwrap().clone();
    let loss_fn = BinaryCrossEntropyLossConfig::new().init(&device);
    
    for item in test_dataset {
        let prediction = model.forward(item.graph_1.clone(), item.graph_2.clone());
        let loss = loss_fn.forward(prediction.clone(), Tensor::from_ints([if item.label { 1 } else { 0 }], &device));
        average_loss += loss.into_scalar().into();
        match (prediction.into_scalar().into() > 0.5, item.label) {
            (true, true) => true_positive += 1,
            (false, false) => true_negative += 1,
            (true, false) => false_positive += 1,
            (false, true) => false_negative += 1,
        }
    }

    ValidationOutput {
        average_loss: average_loss / test_dataset.len() as f64,
        true_positive,
        true_negative,
        false_positive,
        false_negative,
    }
}
