use fxhash::FxHashSet;
use itertools::Itertools;
use crate::{dataset_loader::PlagiarismDataset, plagiarism_runner::{Config, PlagiarismOutput, PlagiarismRunner}};

pub struct EvaluationResult {
    pub plagiarized_pairs: FxHashSet<(String, String)>,
    pub authentic_pairs: FxHashSet<(String, String)>,
    pub output: PlagiarismOutput,
}

fn run_full_cpp_dataset(config: &Config, dataset: &'static PlagiarismDataset) -> Result<EvaluationResult, Box<dyn std::error::Error>> {
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
    let output = runner.run_comparison(
        Some(&config),
        &all_pairs,
        Some("cpp"));
    return output.map(|output| EvaluationResult {
        plagiarized_pairs,
        authentic_pairs,
        output,
    });
}

pub fn average_error_for_config(config: &Config, dataset: &'static PlagiarismDataset) -> f64 {
    let Ok(EvaluationResult { plagiarized_pairs, authentic_pairs, output }) = run_full_cpp_dataset(config, dataset) else {
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
    return total_error / (plagiarized_pairs.len() + authentic_pairs.len()) as f64;
}

fn accuracy_with_threshold(evaluation: &EvaluationResult, threshold: f64) -> f64 {
    let EvaluationResult { plagiarized_pairs, authentic_pairs, output } = evaluation;
    
    let correct = output.results.iter()
        .map(|r| {
            let similarity = r.similarity.as_ref().map(|s| s.combined).unwrap_or(0.0);
            let predicted_plag = similarity >= threshold;
            let is_plag = plagiarized_pairs.contains(&(r.left_file.clone(), r.right_file.clone()));
            if predicted_plag == is_plag {
                return 1;
            } else {
                return 0;
            }
        })
        .sum::<i32>();
    return correct as f64 / (output.results.len()) as f64;
}

pub fn accuracy_with_threshold_for_config(config: &Config, dataset: &'static PlagiarismDataset, threshold: f64) -> f64 {
    let Ok(evaluation) = run_full_cpp_dataset(config, dataset) else {
        return f64::NEG_INFINITY;
    };
    return accuracy_with_threshold(&evaluation, threshold);
}

pub fn accuracy_threshold_curve_for_config(config: &Config, dataset: &'static PlagiarismDataset) -> Vec<(f64, f64)> {
    let Ok(evaluation) = run_full_cpp_dataset(config, dataset) else {
        return Vec::new();
    };

    let mut threshold_accurracy_list = Vec::new();
    let thresholds = (1..20).map(|i| i as f64 * 5.0 / 100.0).collect_vec();
    for threshold in thresholds {
        let accuracy = accuracy_with_threshold(&evaluation, threshold);
        threshold_accurracy_list.push((threshold, accuracy));
    }
    return threshold_accurracy_list;
}
