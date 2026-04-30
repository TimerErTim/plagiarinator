use data_loading::dataset_loader::load_dataset;

fn main() {
    let dataset_path = "datasets/c_cpp_plagiarism";
    let dataset = load_dataset(dataset_path).unwrap();
    let cpp_dataset = dataset.cpp_dataset;
    dbg!(cpp_dataset.plagiarized_pairs.iter().collect::<Vec<_>>());
    dbg!(cpp_dataset.plagiarized_pairs.len());
    dbg!(cpp_dataset
        .authentic_pairs
        .iter()
        .take(10)
        .collect::<Vec<_>>());
}
