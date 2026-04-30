use std::path::{Path, PathBuf};

use cpp_dataset::load_cpp_dataset;
use rand::Rng;

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

    pub(crate) fn add_plagiarized_pair(
        &mut self,
        left_path: impl Into<PathBuf>,
        right_path: impl Into<PathBuf>,
    ) {
        let left_path = left_path.into();
        let right_path = right_path.into();
        self.plagiarized_pairs.push(FilePair {
            left_path,
            right_path,
        });
    }

    pub(crate) fn add_authentic_pair(
        &mut self,
        left_path: impl Into<PathBuf>,
        right_path: impl Into<PathBuf>,
    ) {
        let left_path = left_path.into();
        let right_path = right_path.into();
        self.authentic_pairs.push(FilePair {
            left_path,
            right_path,
        });
    }

    pub fn split_dataset(
        &self,
        ratio: f32,
        rng: &mut impl Rng,
    ) -> (LanguageDataset, LanguageDataset) {
        use rand::seq::SliceRandom;

        // Clone and shuffle pairs
        let mut plagiarized_pairs = self.plagiarized_pairs.clone();
        let mut authentic_pairs = self.authentic_pairs.clone();
        plagiarized_pairs.shuffle(rng);
        authentic_pairs.shuffle(rng);

        // Calculate split indices
        let split_plag = ((plagiarized_pairs.len() as f32) * ratio).round() as usize;
        let split_auth = ((authentic_pairs.len() as f32) * ratio).round() as usize;

        // Split vectors
        let (plag_train, plag_test) = plagiarized_pairs.split_at(split_plag);
        let (auth_train, auth_test) = authentic_pairs.split_at(split_auth);

        // Create train and test datasets
        let train_dataset = LanguageDataset {
            lang_name: self.lang_name.clone(),
            plagiarized_pairs: plag_train.to_vec(),
            authentic_pairs: auth_train.to_vec(),
        };

        let test_dataset = LanguageDataset {
            lang_name: self.lang_name.clone(),
            plagiarized_pairs: plag_test.to_vec(),
            authentic_pairs: auth_test.to_vec(),
        };

        (train_dataset, test_dataset)
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
    Ok(PlagiarismDataset {
        cpp_dataset,
        python_dataset,
    })
}
