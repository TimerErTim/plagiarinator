use std::path::{Path, PathBuf};

use cpp_dataset::load_cpp_dataset;

mod cpp_dataset;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct FilePair {
    pub left_path: PathBuf,
    pub right_path: PathBuf,
}

#[derive(Debug, Clone)]
pub struct LanguageDataset {
    pub lang_name: String,
    pub plagiarized_pairs: Vec<FilePair>,
    pub authentic_pairs: Vec<FilePair>,
}

impl LanguageDataset {
    pub fn new(lang_name: impl Into<String>) -> Self {
        Self {
            lang_name: lang_name.into(),
            plagiarized_pairs: Vec::new(),
            authentic_pairs: Vec::new(),
        }
    }

    pub fn add_plagiarized_pair(&mut self, left_path: impl Into<PathBuf>, right_path: impl Into<PathBuf>) {
        let left_path = left_path.into();
        let right_path = right_path.into();
        self.plagiarized_pairs.push(FilePair { left_path, right_path });
    }

    pub fn add_authentic_pair(&mut self, left_path: impl Into<PathBuf>, right_path: impl Into<PathBuf>) {
        let left_path = left_path.into();
        let right_path = right_path.into();
        self.authentic_pairs.push(FilePair { left_path, right_path });
    }
}

#[derive(Debug, Clone)]
pub struct PlagiarismDataset {
    pub cpp_dataset: LanguageDataset,
    pub python_dataset: LanguageDataset,
}

pub fn load_dataset(
    cpp_dataset_root: impl AsRef<Path>,
    // python_dataset_root: impl AsRef<Path>,
) -> Result<PlagiarismDataset, String> {
    let cpp_dataset = load_cpp_dataset(cpp_dataset_root)?;
    let python_dataset = LanguageDataset::new("python");
    Ok(PlagiarismDataset { cpp_dataset, python_dataset })
}
