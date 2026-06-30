use data_loading::{full_cpp_dataset, train_test_cpp_dataset};
use graph_deeplearning::{load_trained_model, loading::make_testset_loader, validation::validate};

pub fn main() {
    type InferenceBackend = burn::backend::Flex;
    let device = burn::backend::flex::FlexDevice::default();

    let (_, cpp_test_dataset) = train_test_cpp_dataset().unwrap();
    let test_loader = make_testset_loader::<InferenceBackend>(
        cpp_test_dataset.plagiarized_pairs,
        cpp_test_dataset.authentic_pairs,
        &device,
    );
    let model = load_trained_model::<InferenceBackend>(&device);
    let validation_output = validate(model, test_loader.iter());
    println!(
        "{}",
        serde_json::to_string_pretty(&validation_output).unwrap()
    );
}
