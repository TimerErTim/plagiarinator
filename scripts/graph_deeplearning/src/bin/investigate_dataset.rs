use data_loading::{dataset_loader::load_dataset, full_cpp_dataset};

fn main() {
    let dataset = full_cpp_dataset().unwrap();
    dbg!(dataset.plagiarized_pairs.len());
    dbg!(dataset.authentic_pairs.len());

    let (trainset, testset) = dataset.train_test_split();
    dbg!(trainset.plagiarized_pairs.len());
    dbg!(trainset.authentic_pairs.len());
    dbg!(testset.plagiarized_pairs.len());
    dbg!(testset.authentic_pairs.len());
}
