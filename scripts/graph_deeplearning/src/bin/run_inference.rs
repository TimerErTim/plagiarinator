use burn::backend::Autodiff;
use burn_store::ModuleSnapshot;
use decider_model::data::{analyze_plagiarism, parse_cpp_file};
use graph_deeplearning::load_trained_model;

pub fn main() {
    use std::env;

    type InferenceBackend = burn::backend::ndarray::NdArray;
    let device = burn::backend::ndarray::NdArrayDevice::default();

    let args: Vec<String> = env::args().collect();

    if args.len() != 3 {
        eprintln!(
            "Usage: {} <item1> <item2>",
            args.first().map(|s| s.as_str()).unwrap_or("run_inference")
        );
        std::process::exit(1);
    }

    let item1 = &args[1];
    let item2 = &args[2];

    let model = load_trained_model::<Autodiff<InferenceBackend>>(&device);

    let file1 = parse_cpp_file(item1);
    let file2 = parse_cpp_file(item2);

    let analysis = analyze_plagiarism(file1, file2, model).unwrap();
    println!("{}", serde_json::to_string_pretty(&analysis).unwrap());
}
