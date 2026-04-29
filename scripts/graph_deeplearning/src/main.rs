use burn::{
    backend::Autodiff,
    grad_clipping::GradientClippingConfig,
    module::{AutodiffModule, Module},
    nn::loss::BinaryCrossEntropyLossConfig,
    optim::{decay::WeightDecayConfig, AdamConfig, GradientsParams, Optimizer},
    prelude::Backend,
    Tensor,
};
use data_loading::dataset_loader::load_dataset;

use graph_deeplearning::{
    loading::{chunked_iter, make_testset_loader, make_trainset_loader, PrefetchIterator},
    model::PlagiarismTrainItem,
    nn::{PlagiarismDecider, PlagiarismDeciderConfig, PlagiarismDeciderLayerConfig},
};
use rand::SeedableRng;

pub fn main() {
    // Backend type selection
    type InfBackend = std::cfg_select! {
        feature = "cuda" => {
            burn::backend::Cuda
        } feature = "cubecl-cpu" => {
            burn::backend::Cpu
        } feature = "torch" => {
            burn::backend::LibTorch
        } _ => {
            burn::backend::NdArray
        }
    };
    type AdBackend = Autodiff<InfBackend>;
    let device = std::cfg_select! {
        feature = "cuda" => {
            burn::backend::cuda::CudaDevice::default()
        } feature = "cubecl-cpu" => {
            burn::backend::cpu::CpuDevice::default()
        } feature = "torch" => {
            burn::backend::tch::LibTorchDevice::default()
        } _ => {
            burn::backend::ndarray::NdArrayDevice::default()
        }
    };
    println!("graph_deeplearning: using Burn device {device:?}");

    let seed = 36;
    let batch_size = 4;
    let validation_interval = 10;
    let learning_rate = 0.001;
    let mut rng = rand::rngs::SmallRng::seed_from_u64(seed);
    AdBackend::seed(&device, seed);

    let mut dataset = load_dataset("datasets/c_cpp_plagiarism").unwrap();
    // Make equal split of plagiarized and authentic pairs
    let len = dataset
        .cpp_dataset
        .plagiarized_pairs
        .len()
        .max(dataset.cpp_dataset.authentic_pairs.len());
    let rem_plagiarized_pairs = dataset
        .cpp_dataset
        .plagiarized_pairs
        .iter()
        .skip(len)
        .cloned()
        .collect::<Vec<_>>();
    let rem_authentic_pairs = dataset
        .cpp_dataset
        .authentic_pairs
        .iter()
        .skip(len)
        .cloned()
        .collect::<Vec<_>>();
    dataset.cpp_dataset.plagiarized_pairs = dataset
        .cpp_dataset
        .plagiarized_pairs
        .into_iter()
        .take(len)
        .collect();
    dataset.cpp_dataset.authentic_pairs = dataset
        .cpp_dataset
        .authentic_pairs
        .into_iter()
        .take(len)
        .collect();

    let (cpp_train_dataset, mut cpp_test_dataset) =
        dataset.cpp_dataset.split_dataset(0.8, &mut rng);
    cpp_test_dataset
        .plagiarized_pairs
        .extend(rem_plagiarized_pairs);
    cpp_test_dataset.authentic_pairs.extend(rem_authentic_pairs);
    let num_train_items =
        cpp_train_dataset.plagiarized_pairs.len() + cpp_train_dataset.authentic_pairs.len();
    let cpp_test_items = make_testset_loader::<InfBackend>(
        cpp_test_dataset.plagiarized_pairs,
        cpp_test_dataset.authentic_pairs,
        &device,
    );
    let train_loader = make_trainset_loader::<AdBackend>(
        cpp_train_dataset.plagiarized_pairs,
        cpp_train_dataset.authentic_pairs,
        rng.clone(),
        &device,
    );

    let train_loader = PrefetchIterator::new(train_loader, batch_size * 2);
    let batched_loader = chunked_iter(train_loader, batch_size);

    // init model
    let model_config = PlagiarismDeciderConfig::new(
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
    );
    let mut model = model_config.init::<AdBackend>(&device);
    println!("model: {model}");
    println!("num_train_items: {num_train_items}");

    // init optimizer and loss function
    let mut optimizer = AdamConfig::new()
        .with_weight_decay(Some(WeightDecayConfig::new(0.01)))
        .with_grad_clipping(Some(GradientClippingConfig::Value(1.0)))
        .init();
    let loss_fn = BinaryCrossEntropyLossConfig::new().init(&device);

    let mut total_loss = Tensor::zeros([1], &device);
    for (idx, batch) in batched_loader.enumerate() {
        if idx % validation_interval == 0 {
            let validation_output = validate(model.valid(), cpp_test_items.iter());
            println!(
                "step {idx}, training_loss: {:.02}, validation_output: {validation_output:?}",
                (total_loss.clone() / (validation_interval as f64)).into_scalar()
            );
            total_loss = total_loss.slice_fill([0], 0.0);
        }

        let targets = Tensor::from_data(
            batch
                .iter()
                .map(|item| if item.label { 1 } else { 0 })
                .collect::<Vec<_>>()
                .as_slice(),
            &device,
        );
        let predictions = Tensor::cat(
            batch
                .into_iter()
                .map(|item| model.forward(item.graph_1, item.graph_2))
                .collect(),
            0,
        );

        println!("predictions: {}\ntargets: {}", predictions.to_data(), targets.to_data());
        let loss = loss_fn.forward(predictions, targets);
        total_loss = total_loss.add(loss.clone().inner());
        let mut grads = loss.backward();
        let param_grads = GradientsParams::from_module(&mut grads, &model);
        model = optimizer.step(learning_rate, model, param_grads);
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

pub fn validate<B: Backend>(
    model: PlagiarismDecider<B>,
    test_dataset: impl Iterator<Item = PlagiarismTrainItem<B>>,
) -> ValidationOutput
where
    B::FloatElem: Into<f64>,
{
    let mut true_positive = 0;
    let mut true_negative = 0;
    let mut false_positive = 0;
    let mut false_negative = 0;
    let device = model.devices().first().unwrap().clone();
    let loss_fn = BinaryCrossEntropyLossConfig::new().init(&device);
    let mut predictions = Vec::with_capacity(test_dataset.size_hint().0);
    let mut targets = Vec::with_capacity(test_dataset.size_hint().0);

    for item in test_dataset {
        let prediction = model.forward(item.graph_1.clone(), item.graph_2.clone());
        predictions.push(prediction.clone());
         targets.push(Tensor::from_ints([if item.label { 1 } else { 0 }], &device),
        );
        match (prediction.into_scalar().into() > 0.5, item.label) {
            (true, true) => true_positive += 1,
            (false, false) => true_negative += 1,
            (true, false) => false_positive += 1,
            (false, true) => false_negative += 1,
        }
    }

    ValidationOutput {
        average_loss: loss_fn.forward(Tensor::cat(predictions, 0), Tensor::cat(targets, 0)).into_scalar().into(),
        true_positive,
        true_negative,
        false_positive,
        false_negative,
    }
}
