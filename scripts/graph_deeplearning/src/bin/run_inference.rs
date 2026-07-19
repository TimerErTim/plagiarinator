use burn::backend::Autodiff;
use decider_model::data::{analyze_plagiarism, analyze_plagiarism_all_pairs, parse_cpp_file};
use graph_deeplearning::load_trained_model;

pub fn main() {
    use std::env;

    type InferenceBackend = burn::backend::Flex;
    let device = burn::backend::flex::FlexDevice::default();

    let args: Vec<String> = env::args().collect();

    if args.len() < 3 {
        eprintln!(
            "Usage: {} <item1> <item2> [item3] [item4] [...]",
            args.first().map(|s| s.as_str()).unwrap_or("run_inference")
        );
        std::process::exit(1);
    }

    let model = load_trained_model::<Autodiff<InferenceBackend>>(&device);
    
    let items = args[1..].iter().map(|s| s.as_str()).collect::<Vec<_>>();
    let mut 
    files = items.iter().map(|s| parse_cpp_file(s)).collect::<Vec<_>>();

    if files.len() == 2 {
        let analysis = analyze_plagiarism(files.pop().unwrap(), files.pop().unwrap(), model, true).unwrap();
        println!("{}", serde_json::to_string_pretty(&analysis).unwrap());
    } else {
        let analysis = analyze_plagiarism_all_pairs(&files, model).unwrap();
        println!("{}", serde_json::to_string_pretty(&analysis).unwrap());
    }
}
