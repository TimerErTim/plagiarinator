use crate::dataset_loader::{
    load_cpp_dataset, load_self_sourced_llm_dataset, LanguageDataset,
};

pub mod dataset_loader;
pub mod split;

pub fn full_cpp_dataset() -> Result<LanguageDataset, String> {
    let datasets_root = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .join("..")
        .join("..")
        .join("datasets");

    let homework_root = datasets_root.join("programming-homework-dataset-plagiarism-detection");
    let self_sourced_root = datasets_root.join("self-sourced-llm");

    let homework_dataset = load_cpp_dataset(&homework_root)?;
    let self_sourced_dataset = load_self_sourced_llm_dataset(&self_sourced_root)?;

    Ok(homework_dataset.merge(self_sourced_dataset))
}

#[cfg(test)]
mod tests {
    use super::full_cpp_dataset;
    use crate::dataset_loader::load_self_sourced_llm_dataset;

    #[test]
    fn full_cpp_dataset_loads_self_sourced_subset() {
        let dataset = full_cpp_dataset().expect("full_cpp_dataset should load");
        let self_sourced = load_self_sourced_llm_dataset(
            std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
                .join("..")
                .join("..")
                .join("datasets")
                .join("self-sourced-llm"),
        )
        .expect("self-sourced dataset should load");

        assert_eq!(self_sourced.plagiarized_pairs.len(), 640);
        assert_eq!(self_sourced.authentic_pairs.len(), 12720);
        assert!(dataset.plagiarized_pairs.len() > self_sourced.plagiarized_pairs.len());
        assert!(dataset.authentic_pairs.len() > self_sourced.authentic_pairs.len());
    }
}
