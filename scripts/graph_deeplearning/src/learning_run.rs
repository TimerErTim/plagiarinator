use std::path::{Path, PathBuf};

use burn::tensor::backend::Backend;
use burn_store::ModuleSnapshot;
use serde::{Deserialize, Serialize, de::DeserializeOwned};

pub struct RunningTraining<I, M, O> {
    pub model: M,
    pub optimizer: O,
    pub meta: I,
    step: usize,
    run_timestamp: String,
    checkpoint_dir: PathBuf,
}

#[derive(Serialize, Deserialize)]
struct StoredMetaInformation<I> {
    pub run_timestamp: String,
    pub meta: I,
}

struct CheckpointFiles {
    model: PathBuf,
    optimizer: PathBuf,
    meta_information: PathBuf,
}

impl<I, M, O> RunningTraining<I, M, O> {
    pub fn new(model: M, optimizer: O, meta: I, checkpoint_dir: PathBuf) -> Self {
        Self { model, optimizer, meta, step: 0, run_timestamp: chrono::Utc::now().format("%Y%m%d_%H%M%S").to_string(), checkpoint_dir }
    }
    
    pub fn run_timestamp(&self) -> String {
        self.run_timestamp.clone()
    }
    
    pub fn current_step(&self) -> usize {
        self.step
    }
    
    pub fn load_checkpoint<B1: Backend, B2: Backend>(mut model: M, mut optimizer: O, checkpoint_dir: impl AsRef<Path>, step: usize) -> Result<Self, CheckpointError>
    where M: ModuleSnapshot<B1>, O: ModuleSnapshot<B2>, StoredMetaInformation<I>: DeserializeOwned {
        let files = Self::checkpoint_files(checkpoint_dir.as_ref(), step);
        
        let mut model_store = burn::store::BurnpackStore::from_file(files.model);
        let mut optimizer_store = burn::store::BurnpackStore::from_file(files.optimizer);
        let meta_information: StoredMetaInformation<I> = serde_json::from_reader(std::fs::File::open(files.meta_information).map_err(|e| CheckpointError::MetaInformationLoadError(e.to_string()))?).map_err(|e| CheckpointError::MetaInformationLoadError(e.to_string()))?;
        
        let _ = model.load_from(&mut model_store).map_err(|e| CheckpointError::ModelLoadError(e.to_string()))?;
        let _ = optimizer.load_from(&mut optimizer_store).map_err(|e| CheckpointError::OptimizerLoadError(e.to_string()))?;

        Ok(Self {
            model,
            optimizer,
            meta: meta_information.meta,
            step,
            run_timestamp: meta_information.run_timestamp,
            checkpoint_dir: checkpoint_dir.as_ref().to_path_buf(),
        })
    }

    pub fn load_latest_checkpoint<B1: Backend, B2: Backend>(model: M, optimizer: O, checkpoint_dir: impl AsRef<Path>) -> Result<Self, CheckpointError>
    where M: ModuleSnapshot<B1>, O: ModuleSnapshot<B2>, StoredMetaInformation<I>: DeserializeOwned {
        // Find latest checkpoint by sorting by step and taking the last one, with safe error handling
        let checkpoints = std::fs::read_dir(checkpoint_dir.as_ref())
            .map_err(|e| CheckpointError::LatestCheckpointNotFound(e.to_string()))?;

        let mut max_step: Option<usize> = None;

        for entry_result in checkpoints {
            let entry = match entry_result {
                Ok(e) => e,
                Err(e) => return Err(CheckpointError::LatestCheckpointNotFound(e.to_string())),
            };
            let path = entry.path();
            let file_name_os = match path.file_name() {
                Some(name) => name,
                None => continue,
            };
            let file_name = match file_name_os.to_str() {
                Some(s) => s,
                None => continue,
            };
            // Should match e.g. "model_1234.bpk" or "optimizer_5678.bpk"
            // We only care about "model_{step}.bpk"
            if file_name.starts_with("model_") && file_name.ends_with(".bpk") {
                let without_prefix = &file_name["model_".len()..file_name.len() - ".bpk".len()];
                if let Ok(step) = without_prefix.parse::<usize>() {
                    max_step = match max_step {
                        Some(existing) => Some(existing.max(step)),
                        None => Some(step),
                    };
                }
            }
        }

        let latest_checkpoint = match max_step {
            Some(step) => step,
            None => return Err(CheckpointError::LatestCheckpointNotFound("No valid checkpoints found".to_string())),
        };

        Self::load_checkpoint(model, optimizer, checkpoint_dir.as_ref(), latest_checkpoint)
    }

    pub fn save_checkpoint<B1: Backend, B2: Backend>(&self) -> Result<(), CheckpointError>
    where M: ModuleSnapshot<B1>, O: ModuleSnapshot<B2>, I: Serialize + Clone {
        std::fs::create_dir_all(&self.checkpoint_dir).map_err(|e| CheckpointError::ModelSaveError(e.to_string()))?;
        let files = Self::checkpoint_files(&self.checkpoint_dir, self.step);
        let mut model_store = burn::store::BurnpackStore::from_file(files.model).overwrite(true);
        let mut optimizer_store = burn::store::BurnpackStore::from_file(files.optimizer).overwrite(true);
        let stored_meta_information = StoredMetaInformation::<I> {
            run_timestamp: self.run_timestamp.clone(),
            meta: self.meta.clone(),
        };
        let serialized_meta_information = serde_json::to_string(&stored_meta_information).map_err(|e| CheckpointError::MetaInformationSaveError(e.to_string()))?;
        let serialized_plain_meta = serde_json::to_string(&self.meta).map_err(|e| CheckpointError::MetaInformationSaveError(e.to_string()))?;

        model_store = model_store.metadata("run_timestamp", self.run_timestamp.clone()).metadata("step", self.step.to_string()).metadata("meta", serialized_plain_meta.clone());
        optimizer_store = optimizer_store.metadata("run_timestamp", self.run_timestamp.clone()).metadata("step", self.step.to_string()).metadata("meta", serialized_plain_meta);
        
        self.model.save_into(&mut model_store).map_err(|e| CheckpointError::ModelSaveError(e.to_string()))?;
        self.optimizer.save_into(&mut optimizer_store).map_err(|e| CheckpointError::OptimizerSaveError(e.to_string()))?;
        std::fs::write(files.meta_information, serialized_meta_information).map_err(|e| CheckpointError::MetaInformationSaveError(e.to_string()))?;

        Ok(())
    }

    fn checkpoint_files(checkpoint_dir: impl AsRef<Path>, step: usize) -> CheckpointFiles {
        CheckpointFiles {
            model: checkpoint_dir.as_ref().join(format!("model_{step}.bpk",)),
            optimizer: checkpoint_dir.as_ref().join(format!("optimizer_{step}.bpk")),
            meta_information: checkpoint_dir.as_ref().join(format!("meta_information_{step}.json")),
        }
    }
}

#[derive(Debug, thiserror::Error)]
pub enum CheckpointError {
    #[error("Failed to find latest checkpoint: {0}")]
    LatestCheckpointNotFound(String),
    #[error("Failed to load model: {0}")]
    ModelLoadError(String),
    #[error("Failed to load optimizer: {0}")]
    OptimizerLoadError(String),
    #[error("Failed to load meta information: {0}")]
    MetaInformationLoadError(String),
    #[error("Failed to save model: {0}")]
    ModelSaveError(String),
    #[error("Failed to save optimizer: {0}")]
    OptimizerSaveError(String),
    #[error("Failed to save meta information: {0}")]
    MetaInformationSaveError(String),
}

