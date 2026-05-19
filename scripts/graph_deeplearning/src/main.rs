use std::{fs::File, io::Write, path::PathBuf};

use burn::{
    Tensor, backend::Autodiff, grad_clipping::GradientClippingConfig, module::{AutodiffModule, Module}, nn::loss::BinaryCrossEntropyLossConfig, optim::{AdamConfig, GradientsParams, Optimizer, decay::WeightDecayConfig}, prelude::Backend, record::{BinGzFileRecorder, FullPrecisionSettings, NamedMpkGzFileRecorder, Recorder}, tensor::{BasicOps, cast::ToElement}
};
use burn_store::ModuleStore;
use data_loading::dataset_loader::load_dataset;

mod loading;
mod logging;
use logging::create_tensorboard_logger;
use decider_model::{PlagiarismDecider, data::{analyze_plagiarism, parse_cpp_file}, init_model};
use rand::SeedableRng;

use crate::loading::{PlagiarismTrainItem, PrefetchIterator, chunked_iter, make_testset_loader, make_trainset_loader};

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

    let seed = 41;
    let batch_size = 8;
    let validation_interval = 50;
    let checkpoint_interval = 100;
    let learning_rate = 0.002;
    let mut rng = rand::rngs::SmallRng::seed_from_u64(seed);
    AdBackend::seed(&device, seed);
    let run_timestamp = chrono::Utc::now().format("%Y%m%d_%H%M%S").to_string();

    let checkpoint_dir = PathBuf::from(format!("out/deep_learning/run_{run_timestamp}/checkpoints"));
    std::fs::create_dir_all(&checkpoint_dir).unwrap();
    let analysis_dir = PathBuf::from(format!("out/deep_learning/run_{run_timestamp}/analysis"));
    std::fs::create_dir_all(&analysis_dir).unwrap();
    let mut tensorboard_logger = create_tensorboard_logger("unknown", &run_timestamp);

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
        cpp_test_dataset.plagiarized_pairs.clone(),
        cpp_test_dataset.authentic_pairs.clone(),
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
    let mut model = init_model::<AdBackend>(&device);
    println!("model: {model}");
    println!("num_train_items: {num_train_items}");

    // init optimizer and loss function
    let mut optimizer = AdamConfig::new()
        .with_weight_decay(Some(WeightDecayConfig::new(0.01)))
        .with_grad_clipping(Some(GradientClippingConfig::Value(1.0)))
        .init();
    let loss_fn = BinaryCrossEntropyLossConfig::new().init(&device);

    for (step, batch) in batched_loader.enumerate() {
        if step % checkpoint_interval == 0 {
            // Save model
            let mut store = burn::store::BurnpackStore::from_file(checkpoint_dir.join(format!("model_{}_{step}.bpk", &run_timestamp)))
                .metadata("run_timestamp", run_timestamp.clone())
                .metadata("step", step.to_string());
            store.collect_from(&model).unwrap();

            let recorder = NamedMpkGzFileRecorder::<FullPrecisionSettings>::new();
            model.clone().save_file(checkpoint_dir.join(format!("model_{}_{step}", &run_timestamp)), &recorder).unwrap();
        }

        if step % validation_interval == 0 {
            println!("\nValidating...");
            let validation_output = validate(model.valid(), cpp_test_items.iter());
            tensorboard_logger.log_scalar("validation/loss", validation_output.average_loss, step as u64);
            tensorboard_logger.log_scalar("validation/precision", validation_output.precision, step as u64);
            tensorboard_logger.log_scalar("validation/recall", validation_output.recall, step as u64);
            tensorboard_logger.log_scalar("validation/f1_score", validation_output.f1_score, step as u64);

            // Sample an item for analysis
            let item = cpp_test_dataset.plagiarized_pairs.iter().find(|i| i.left_path != i.right_path).cloned().unwrap();
            let analyzed_output = analyze_plagiarism(parse_cpp_file(item.left_path), parse_cpp_file(item.right_path), model.clone()).unwrap();
            serde_json::to_writer_pretty(File::create(analysis_dir.join(format!("analyzed_output_{step}.json"))).unwrap(), &analyzed_output).unwrap();
            tensorboard_logger.log_text("validation/analyzed_output", &serde_json::to_string_pretty(&analyzed_output).unwrap(), step as u64);
        }

        let targets_bool = batch.iter().map(|item| item.label).collect::<Vec<_>>();

        // Loss calculation
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
        let predictions_inner = predictions.clone().inner();
        let loss = loss_fn.forward(predictions, targets);
        
        // Logging
        let stats = statistics_from_training(loss.clone().into_scalar().to_f64(), predictions_inner, targets_bool);
        print!(".");
        tensorboard_logger.log_scalar("training/loss", stats.average_loss, step as u64);
        tensorboard_logger.log_scalar("training/precision", stats.precision, step as u64);
        tensorboard_logger.log_scalar("training/recall", stats.recall, step as u64);
        tensorboard_logger.log_scalar("training/f1_score", stats.f1_score, step as u64);
        std::io::stdout().flush().unwrap();
        tensorboard_logger.flush();

        // Optimization step
        let mut grads = loss.backward();
        let param_grads = GradientsParams::from_module(&mut grads, &model);
        model = optimizer.step(learning_rate, model, param_grads);
    }
}

#[derive(Debug)]
pub struct StatisticsOutput {
    pub average_loss: f64,
    pub true_positive: usize,
    pub true_negative: usize,
    pub false_positive: usize,
    pub false_negative: usize,
    pub precision: f64,
    pub recall: f64,
    pub f1_score: f64,
}

pub fn statistics_from_training<B: Backend>(
    loss: f64,
    predictions: Tensor<B, 1>,
    targets: Vec<bool>,
) -> StatisticsOutput {
    let mut true_positive = 0;
    let mut true_negative = 0;
    let mut false_positive = 0;
    let mut false_negative = 0;
    for (prediction, target) in predictions.iter_dim(0).zip(targets.iter()) {
        match (prediction.into_scalar().to_f64() > 0.5, *target) {
            (true, true) => true_positive += 1,
            (false, false) => true_negative += 1,
            (true, false) => false_positive += 1,
            (false, true) => false_negative += 1,
        }
    }

    let precision = if true_positive + false_positive > 0 {
        true_positive as f64 / (true_positive + false_positive) as f64
    } else {
        0.0
    };
    let recall = if true_positive + false_negative > 0 {
        true_positive as f64 / (true_positive + false_negative) as f64
    } else {
        0.0
    };
    let f1_score = if precision + recall > 0.0 {
        2.0 * precision * recall / (precision + recall)
    } else {
        0.0
    };

    StatisticsOutput {
        average_loss: loss,
        true_positive,
        true_negative,
        false_positive,
        false_negative,
        precision,
        recall,
        f1_score,
    }
}

pub fn validate<B: Backend>(
    model: PlagiarismDecider<B>,
    test_dataset: impl Iterator<Item = PlagiarismTrainItem<B>>,
) -> StatisticsOutput
{
    let device = model.devices().first().unwrap().clone();
    let loss_fn = BinaryCrossEntropyLossConfig::new().init(&device);
    let mut predictions = Vec::with_capacity(test_dataset.size_hint().0);
    let mut targets = Vec::with_capacity(test_dataset.size_hint().0);
    let mut targets_bool = Vec::with_capacity(test_dataset.size_hint().0);

    for item in test_dataset {
        let prediction = model.forward(item.graph_1.clone(), item.graph_2.clone());
        predictions.push(prediction.clone());
        targets.push(Tensor::from_ints([if item.label { 1 } else { 0 }], &device));
        targets_bool.push(item.label);
    }
    let predictions = Tensor::cat(predictions, 0);
    let loss = loss_fn.forward(predictions.clone(), Tensor::cat(targets, 0)).into_scalar().to_f64();

    return statistics_from_training(loss, predictions, targets_bool);
}
