mod dataset_loader;
mod plagiarism_runner;
use std::path::Path;

use dataset_loader::load_dataset;
use plagiarism_runner::PlagiarismRunner;

use crate::dataset_loader::PlagiarismDataset;

fn main() {
    let dataset_root = Path::new("datasets/c_cpp_plagiarism");
    let dataset = load_dataset(dataset_root).unwrap();
    let global_dataset: &PlagiarismDataset = Box::leak(Box::new(dataset));

    
    let runner = PlagiarismRunner::new();
    for entry in global_dataset.cpp_dataset.plagiarized_pairs.iter().chain(global_dataset.cpp_dataset.authentic_pairs.iter()) {
        let results = runner.run_comparison(
            None,
            &[(
                entry.left_path.clone().to_string_lossy().to_string(),
                entry.right_path.clone().to_string_lossy().to_string()
            )],
            Some("cpp"));
        println!("Results: {:?}", results);
    }
    println!("radiate_config_optimizer ready");
}
