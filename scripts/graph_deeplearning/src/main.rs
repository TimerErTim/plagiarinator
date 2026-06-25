use std::{fs::File, io::Write, path::PathBuf};

use burn::{
    Tensor, backend::Autodiff, grad_clipping::GradientClippingConfig, module::{AutodiffModule, Module}, nn::loss::BinaryCrossEntropyLossConfig, optim::{AdamConfig, AdamWConfig, GradientsParams, Optimizer, decay::WeightDecayConfig}, prelude::Backend, record::{FullPrecisionSettings, NamedMpkGzFileRecorder}, tensor::{BasicOps, cast::ToElement}
};
use burn_store::ModuleStore;
use data_loading::{dataset_loader::load_dataset, full_cpp_dataset};

mod loading;
mod logging;
mod learning_run;
mod validation;
use decider_model::{
    data::{analyze_plagiarism, parse_cpp_file},
    init_model, PlagiarismDecider,
};
use logging::create_tensorboard_logger;
use rand::SeedableRng;

use crate::{loading::{
    PlagiarismTrainItem, PrefetchIterator, chunked_iter, make_testset_loader, make_trainset_loader,
}, validation::{validate, validation_from_tensor_predictions}};

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
    let learning_rate = 0.005;
    let rng = rand::rngs::SmallRng::seed_from_u64(seed);
    AdBackend::seed(&device, seed);
    let run_timestamp = chrono::Utc::now().format("%Y%m%d_%H%M%S").to_string();

    let checkpoint_dir =
        PathBuf::from(format!("out/deep_learning/run_{run_timestamp}/checkpoints"));
    std::fs::create_dir_all(&checkpoint_dir).unwrap();
    let analysis_dir = PathBuf::from(format!("out/deep_learning/run_{run_timestamp}/analysis"));
    std::fs::create_dir_all(&analysis_dir).unwrap();
    let mut tensorboard_logger = create_tensorboard_logger("unknown", &run_timestamp);

    let dataset = full_cpp_dataset().unwrap();
    let (cpp_train_dataset, cpp_test_dataset) = dataset.train_test_split();
    let num_train_items = cpp_train_dataset.len();
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
            let mut store = burn::store::BurnpackStore::from_file(
                checkpoint_dir.join(format!("model_{}_{step}.bpk", run_timestamp)),
            )
            .metadata("run_timestamp", run_timestamp.clone())
            .metadata("step", step.to_string());
            store.collect_from(&model).unwrap();

            let recorder = NamedMpkGzFileRecorder::<FullPrecisionSettings>::new();
            model
                .clone()
                .save_file(
                    checkpoint_dir.join(format!("model_{}_{step}", run_timestamp)),
                    &recorder,
                )
                .unwrap();
        }

        if step % validation_interval == 0 {
            println!("\nValidating...");
            let validation_output = validate(model.valid(), cpp_test_items.iter());
            tensorboard_logger.log_scalar(
                "validation/loss",
                validation_output.average_loss,
                step as u64,
            );
            tensorboard_logger.log_scalar(
                "validation/precision",
                validation_output.best_classification_statistic.precision,
                step as u64,
            );
            tensorboard_logger.log_scalar(
                "validation/recall",
                validation_output.best_classification_statistic.recall,
                step as u64,
            );
            tensorboard_logger.log_scalar(
                "validation/f1_score",
                validation_output.best_classification_statistic.f1_score,
                step as u64,
            );
            tensorboard_logger.log_scalar(
                "validation/classification_threshold",
                validation_output.best_classification_statistic.threshold,
                step as u64,
            );

            // Sample an item for analysis
            let item = cpp_test_dataset
                .plagiarized_pairs
                .iter()
                .find(|i| i.left_path != i.right_path)
                .cloned()
                .unwrap();
            let analyzed_output = analyze_plagiarism(
                parse_cpp_file(item.left_path),
                parse_cpp_file(item.right_path),
                model.clone(),
            )
            .unwrap();
            serde_json::to_writer_pretty(
                File::create(analysis_dir.join(format!("analyzed_output_{step}.json"))).unwrap(),
                &analyzed_output,
            )
            .unwrap();
            tensorboard_logger.log_text(
                "validation/analyzed_output",
                &serde_json::to_string_pretty(&analyzed_output).unwrap(),
                step as u64,
            );
        }

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
        let loss = loss_fn.forward(predictions.clone(), targets.clone());

        // Logging
        let stats = validation_from_tensor_predictions(
            loss.clone(),
            predictions,
            targets.float(),
        );
        print!(".");
        tensorboard_logger.log_scalar("training/loss", stats.average_loss, step as u64);
        tensorboard_logger.log_scalar("training/precision", stats.best_classification_statistic.precision, step as u64);
        tensorboard_logger.log_scalar("training/recall", stats.best_classification_statistic.recall, step as u64);
        tensorboard_logger.log_scalar("training/f1_score", stats.best_classification_statistic.f1_score, step as u64);
        tensorboard_logger.log_scalar("training/classification_threshold", stats.best_classification_statistic.threshold, step as u64);
        std::io::stdout().flush().unwrap();
        tensorboard_logger.flush();

        // Optimization step
        let mut grads = loss.backward();
        let param_grads = GradientsParams::from_module(&mut grads, &model);
        model = optimizer.step(learning_rate, model, param_grads);
    }
}

