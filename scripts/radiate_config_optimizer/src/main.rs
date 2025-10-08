mod dataset_loader;
mod plagiarism_runner;
mod genetic_evolution;
use std::path::Path;

use dataset_loader::load_dataset;
use fxhash::FxHashSet;
use itertools::Itertools;
use plagiarism_runner::PlagiarismRunner;
use radiate::{EngineExt, Executor, GeneticEngine};

use crate::{dataset_loader::PlagiarismDataset, genetic_evolution::ConfigCodec, plagiarism_runner::Config};

fn evaluate_config(config: Config, dataset: &'static PlagiarismDataset) -> f64 {
    let plagiarized_pairs = fastrand::choose_multiple(dataset.cpp_dataset.plagiarized_pairs.iter(), 100).iter().map(|i| (*i).clone()).collect_vec();
    let authentic_pairs = fastrand::choose_multiple(dataset.cpp_dataset.authentic_pairs.iter(), 100).iter().map(|i| (*i).clone()).collect_vec();
    let plagiarized_pair_set = plagiarized_pairs.iter().map(|i| (i.left_path.clone().to_string_lossy().to_string(), i.right_path.clone().to_string_lossy().to_string())).collect::<FxHashSet<_>>();
    let authentic_pair_set = authentic_pairs.iter().map(|i| (i.left_path.clone().to_string_lossy().to_string(), i.right_path.clone().to_string_lossy().to_string())).collect::<FxHashSet<_>>();

    let all_pairs = plagiarized_pair_set.iter().chain(authentic_pair_set.iter()).map(|i| (i.0.clone(), i.1.clone())).collect::<Vec<_>>();
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
            let target = if plagiarized_pair_set.contains(&(r.left_file.clone(), r.right_file.clone())) {
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

fn main() {
    let dataset_root = Path::new("datasets/c_cpp_plagiarism");
    let dataset = load_dataset(dataset_root).unwrap();
    let global_dataset: &PlagiarismDataset = Box::leak(Box::new(dataset));

    let codec = ConfigCodec;
    let mut engine = GeneticEngine::builder()
    .minimizing()
    .codec(codec)
    .fitness_fn(|config| {
        evaluate_config(config, global_dataset)
    })
    .executor(Executor::WorkerPool)
    .build();

    let result = engine.run(|epoch| {
        println!("Epoch: {:?}", epoch);
        epoch.score().as_f32() < 0.01 || epoch.index() > 1000
    });

    println!("Best solution found: {:?}", result.value());
}
