mod dataset_loader;
mod plagiarism_runner;
mod genetic_evolution;
use std::path::Path;

use dataset_loader::load_dataset;
use fxhash::{FxHashMap, FxHashSet};
use itertools::Itertools;
use plagiarism_runner::PlagiarismRunner;
use radiate::{EngineExt, Executor, GeneticEngine};

use crate::{dataset_loader::PlagiarismDataset, genetic_evolution::ConfigCodec, plagiarism_runner::{Config, PlagiarismOutput}};

fn average_error_for_config(config: Config, dataset: &'static PlagiarismDataset) -> f64 {
    let plagiarized_pairs = dataset.cpp_dataset.plagiarized_pairs
        .iter()
        .map(|i| (
            i.left_path.clone().to_string_lossy().to_string(),
            i.right_path.clone().to_string_lossy().to_string(),
        ))
        .collect::<FxHashSet<_>>();
    let authentic_pairs = dataset.cpp_dataset.authentic_pairs
        .iter()
        .map(|i| (
            i.left_path.clone().to_string_lossy().to_string(),
            i.right_path.clone().to_string_lossy().to_string(),
        )).collect::<FxHashSet<_>>();

    let all_pairs = plagiarized_pairs.iter().chain(authentic_pairs.iter()).map(|i| (i.0.clone(), i.1.clone())).collect::<Vec<_>>();
    let runner = PlagiarismRunner::new();
    let results = runner.run_comparison(
        Some(&config),
        &all_pairs,
        Some("cpp"));

    let Ok(output) = results else {
        return f64::INFINITY;
    };
    let total_error: f64 = output.results.iter()
        .map(|r| {
            let similarity = r.similarity.as_ref().map(|s| s.combined).unwrap_or(0.0);
            let target = if plagiarized_pairs.contains(&(r.left_file.clone(), r.right_file.clone())) {
                1.0
            } else {
                0.0
            };
            (similarity - target).abs()
        })
        .sum();

    // relative error
    total_error / (plagiarized_pairs.len() + authentic_pairs.len()) as f64
}

fn run_full_cpp_dataset(config: Config, dataset: &'static PlagiarismDataset) -> Result<PlagiarismOutput, Box<dyn std::error::Error>> {
    let plagiarized_pairs = dataset.cpp_dataset.plagiarized_pairs
        .iter()
        .map(|i| (
            i.left_path.clone().to_string_lossy().to_string(),
            i.right_path.clone().to_string_lossy().to_string(),
        )).collect::<FxHashSet<_>>();
    let authentic_pairs = dataset.cpp_dataset.authentic_pairs
        .iter()
        .map(|i| (
            i.left_path.clone().to_string_lossy().to_string(),
            i.right_path.clone().to_string_lossy().to_string(),
        )).collect::<FxHashSet<_>>();
    let all_pairs = plagiarized_pairs.iter().chain(authentic_pairs.iter()).map(|i| (i.0.clone(), i.1.clone())).collect::<Vec<_>>();
    let runner = PlagiarismRunner::new();
    let results = runner.run_comparison(
        Some(&config),
        &all_pairs,
        Some("cpp"));
    return results;
}

fn validate_output_using_threshold(output: PlagiarismOutput, threshold: f64, dataset: &'static PlagiarismDataset) -> f64 {
    let plagiarized_pairs = dataset.cpp_dataset.plagiarized_pairs
        .iter()
        .map(|i| (
            i.left_path.clone().to_string_lossy().to_string(),
            i.right_path.clone().to_string_lossy().to_string(),
        )).collect::<FxHashSet<_>>();

    let correct = output.results.iter()
        .map(|r| {
            let similarity = r.similarity.as_ref().map(|s| s.combined).unwrap_or(0.0);
            let predicted_plag = similarity >= threshold;
            let is_plag = plagiarized_pairs.contains(&(r.left_file.clone(), r.right_file.clone()));
            if predicted_plag == is_plag {
                1
            } else {
                0
            }
        })
        .sum::<i32>();

    correct as f64 / (output.results.len()) as f64
}

fn validate_config_thresholds(output: PlagiarismOutput, dataset: &'static PlagiarismDataset) -> Vec<(f64, f64)> {
    let mut threshold_accurracy_list = Vec::new();
    let thresholds = (1..20).map(|i| i as f64 * 5.0 / 100.0).collect_vec();

    for threshold in thresholds {
        let accuracy = validate_output_using_threshold(output.clone(), threshold, dataset);
        threshold_accurracy_list.push((threshold, accuracy));
    }

    threshold_accurracy_list
}

fn run_evolution(dataset: &'static PlagiarismDataset) {
    let codec = ConfigCodec;
    let mut engine = GeneticEngine::builder()
    .minimizing()
    .codec(codec)
    .fitness_fn(move |config| {
        average_error_for_config(config, dataset)
    })
    .executor(Executor::WorkerPool)
    .build();

    let result = engine.run(|epoch| {
        println!("Epoch: {:?}", epoch);
        let output = run_full_cpp_dataset(epoch.value().clone(), dataset);
        if let Ok(output) = output {
            println!("Accuracy at thresholds: {:?}", validate_config_thresholds(output, dataset));
        }
        epoch.score().as_f32() < 0.01 || epoch.index() > 1000
    });

    println!("Best solution found: {:?}", result.value());
}

fn run_best_config(dataset: &'static PlagiarismDataset) {
    let config = std::fs::read("out/best_config.json").unwrap();
    let config: Config = serde_json::from_slice(&config).unwrap();

    let output = run_full_cpp_dataset(config, dataset);
    let Ok(output) = output else {
        return;
    };
    let thresholds = validate_config_thresholds(output, dataset);
    println!("Accuracy at thresholds:\n{thresholds:?}");
}

fn main() {
    let dataset_root = Path::new("datasets/c_cpp_plagiarism");
    let dataset = load_dataset(dataset_root).unwrap();
    let global_dataset: &PlagiarismDataset = Box::leak(Box::new(dataset));

    run_evolution(&global_dataset);
}
