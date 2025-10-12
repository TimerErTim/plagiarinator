use std::{path::PathBuf, sync::{Arc, Mutex}};

use radiate::prelude::*;
use serde::{Deserialize, Serialize};

use crate::{dataset_loader::PlagiarismDataset, evaluation::accuracy_threshold_curve_for_config, plagiarism_runner::Config};

pub struct PeristenceBaseData {
    directory: PathBuf,
    dataset: &'static PlagiarismDataset,
}

impl PeristenceBaseData {
    fn get_checkpoint_path(&self, global_generation: usize) -> PathBuf {
        let generation_dir = self.directory.join("checkpoints/");
        std::fs::create_dir_all(&generation_dir).unwrap();
        generation_dir.join(format!("checkpoint_{}.json", global_generation))
    }

    fn get_best_config_path(&self) -> PathBuf {
        self.directory.join("best_config.json")
    }

    fn get_metrics_path(&self) -> PathBuf {
        self.directory.join("metrics.json")
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Checkpoint {
    global_generation: usize,
    ecosystem: Ecosystem<FloatChromosome>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct BestConfig {
    config: Config,
    score: Score,
    thresholds: Vec<(f64, f64)>,
    best_threshold: f64,
    best_accuracy: f64,
    global_generation: usize,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct Metrics {
    metrics: Vec<GenerationMetrics>,
    latest_generation: usize,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
struct GenerationMetrics {
    thresholds: Vec<f64>,
    accuracies: Vec<f64>,
    engine_metrics: MetricSet,
    best_score: Score,
    global_generation: usize,
}

pub struct PersistingCheckpointer {
    inner: Arc<PeristenceBaseData>,
    loaded_checkpoint: Option<Checkpoint>,
    unloaded: Mutex<bool>,
}

impl PersistingCheckpointer {
    pub fn unloaded(directory: impl Into<PathBuf>, dataset: &'static PlagiarismDataset) -> Self {
        let base_data = PeristenceBaseData { directory: directory.into(), dataset };
        Self {
            inner: Arc::new(base_data),
            loaded_checkpoint: None,
            unloaded: Mutex::new(true),
        }
    }
    
    pub fn load(directory: impl Into<PathBuf>, dataset: &'static PlagiarismDataset) -> Self {
        let base_data = PeristenceBaseData { directory: directory.into(), dataset };
        let dummy_checkpoint = base_data.get_checkpoint_path(0);
        
        Self {
            inner: Arc::new(base_data),
            loaded_checkpoint: Self::load_latest_checkpoint(dummy_checkpoint.parent().unwrap()),
            unloaded: Mutex::new(false),
        }
    }

    pub fn loaded_checkpoint(&self) -> Option<Checkpoint> {
        self.loaded_checkpoint.clone()
    }

    fn calc_global_generation(&self, local_generation: usize) -> usize {
        if let Some(loaded_checkpoint) = &self.loaded_checkpoint {
            loaded_checkpoint.global_generation + local_generation
        } else {
            local_generation
        }
    }

    fn load_latest_checkpoint(checkpoint_directory: impl Into<PathBuf>) -> Option<Checkpoint> {
        let files = std::fs::read_dir(&checkpoint_directory.into()).map_err(|e| format!("Failed to read checkpoint directory: {}", e)).ok()?;
        let mut loaded_checkpoint: Option<Checkpoint> = None;
        for file in files {
            let file = file.ok()?;
            let path = file.path();
            let checkpoint: Checkpoint = serde_json::from_reader(std::fs::File::open(&path).map_err(|e| format!("Failed to open checkpoint file: {}", e)).ok()?).map_err(|e| format!("Failed to read checkpoint file: {}", e)).ok()?;

            if let Some(loaded_checkpoint) = &loaded_checkpoint && loaded_checkpoint.global_generation > checkpoint.global_generation {
                continue;
            }

            loaded_checkpoint = Some(checkpoint);
        }
        loaded_checkpoint
    }

    pub fn persist_checkpoint(&self, ecosystem: &Ecosystem<FloatChromosome>, local_generation: usize) {
        let mut unloaded = self.unloaded.lock().unwrap();
        if *unloaded {
            // Clear the checkpoint directory on first persist if emptied
            let checkpoint_directory = self.inner.get_checkpoint_path(0).parent().unwrap().to_path_buf();
            std::fs::remove_dir_all(checkpoint_directory).unwrap();
        }

        let new_global_generation = self.calc_global_generation(local_generation);
        
        let checkpoint = Checkpoint {
            global_generation: new_global_generation,
            ecosystem: ecosystem.clone(),
        };
        let path = self.inner.get_checkpoint_path(new_global_generation);
        std::fs::write(path, serde_json::to_string(&checkpoint).unwrap()).unwrap();

        *unloaded = false;
    }

    fn persist_best_config(&self, config: &Config, score: &Score, local_generation: usize) {
        let new_global_generation = self.calc_global_generation(local_generation);

        let threshold_accuracies = accuracy_threshold_curve_for_config(config, self.inner.dataset);
        let (best_threshold, best_accuracy) = threshold_accuracies.iter().max_by(|(_, acc_a), (_, acc_b)| acc_a.total_cmp(acc_b)).cloned().unwrap();

        let best_config = BestConfig {
            config: config.clone(),
            score: score.clone(),
            thresholds: threshold_accuracies,
            best_threshold,
            best_accuracy,
            global_generation: new_global_generation,
        };

        let path = self.inner.get_best_config_path();
        std::fs::write(path, serde_json::to_string(&best_config).unwrap()).unwrap();
    }

    fn persist_metrics(&self, config: &Config, score: &Score, metrics: &MetricSet, local_generation: usize) {
        let new_global_generation = self.calc_global_generation(local_generation);

        let previous_metrics = self.inner.get_metrics_path();
        let previous_metrics: Option<Metrics> = try {
            serde_json::from_reader(
                std::fs::File::open(&previous_metrics)
                .map_err(|e| format!("Failed to open previous metrics file: {}", e)).ok()?
            ).map_err(|e| format!("Failed to read previous metrics file: {}", e)).ok()?
        };

        let mut metrics_vec = previous_metrics.map(|m| m.metrics).unwrap_or_default();

        let thresholds_accuracies = accuracy_threshold_curve_for_config(config, self.inner.dataset);
        let (thresholds, accuracies): (Vec<f64>, Vec<f64>) = thresholds_accuracies.into_iter().unzip();
        let new_generation_metrics = GenerationMetrics {
            thresholds: thresholds,
            accuracies: accuracies,
            engine_metrics: metrics.clone(),
            best_score: score.clone(),
            global_generation: new_global_generation,
        };

        let new_metrics = Metrics {
            metrics: metrics_vec,
            latest_generation: new_global_generation,
        };

        let path = self.inner.get_metrics_path();
        serde_json::to_writer(std::fs::File::create(&path).unwrap(), &new_metrics).unwrap();
    }
}

pub struct MetricsEventHandler(Arc<PersistingCheckpointer>);

impl MetricsEventHandler {
    pub fn from(checkpointer: PersistingCheckpointer) -> Self {
        Self(Arc::new(checkpointer))
    }

    pub fn from_arc(checkpointer: &Arc<PersistingCheckpointer>) -> Self {
        Self(Arc::clone(checkpointer))
    }
}

impl EventHandler<EngineEvent<Config>> for MetricsEventHandler {
    fn handle(&mut self, event: Event<EngineEvent<Config>>) {
        let handler = &self.0;
        match event.data() {
            EngineEvent::EpochComplete{index, metrics, best, score} => {
                println!("|{}| Epoch complete: {}", *index, score.as_f32());
                println!("|{}| Best config: {:?}", *index, best);
                handler.persist_metrics(best, score, metrics, *index);
            },
            EngineEvent::EngineImprovement { index, best, score } => {
                println!("|{}| Engine improvement: {}", *index, score.as_f32());
                handler.persist_best_config(best, score, *index);
            },
            _ => {}
        }
    }
}