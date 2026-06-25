use crate::dataset_loader::{LanguageDataset, load_dataset};

pub mod dataset_loader;
pub mod split;

pub fn full_cpp_dataset() -> Result<LanguageDataset, String> {
    let dataset_path = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("..")
        .join("..")
        .join("datasets")
        .join("programming-homework-dataset-plagiarism-detection");
    let dataset = load_dataset(dataset_path.to_str().ok_or("Failed to convert dataset path to string")?)?;
    Ok(dataset.cpp_dataset)
}
