use burn::{backend::Autodiff, prelude::Backend};
use burn_store::ModuleSnapshot;
use graph_deeplearning::{
    get_model_config, loading::parse_cpp_file, model::analyze_plagiarism, nn::PlagiarismDecider,
};

pub fn main() {
    use std::env;

    type InferenceBackend = burn::backend::ndarray::NdArray;
    let device = burn::backend::ndarray::NdArrayDevice::default();

    let args: Vec<String> = env::args().collect();

    if args.len() != 3 {
        eprintln!(
            "Usage: {} <item1> <item2>",
            args.get(0).map(|s| s.as_str()).unwrap_or("run_inference")
        );
        std::process::exit(1);
    }

    let item1 = &args[1];
    let item2 = &args[2];

    let model = load_model::<InferenceBackend>(&device);

    let file1 = parse_cpp_file(item1);
    let file2 = parse_cpp_file(item2);

    let analysis = analyze_plagiarism(file1, file2, model).unwrap();
    println!("{}", serde_json::to_string_pretty(&analysis).unwrap());
}

static MODEL_WEIGHTS: &[u8] = include_bytes!(env!("MODEL_WEIGHTS_PATH"));

fn load_model<B: Backend>(
    device: &<Autodiff<B> as Backend>::Device,
) -> PlagiarismDecider<Autodiff<B>> {
    let mut store = burn::store::BurnpackStore::from_static(MODEL_WEIGHTS);
    let mut model = get_model_config().init::<Autodiff<B>>(device);
    model.load_from(&mut store).unwrap();
    model
}
