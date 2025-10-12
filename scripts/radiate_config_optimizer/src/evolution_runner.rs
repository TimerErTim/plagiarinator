use std::{path::PathBuf, sync::Arc};

use crate::{
    dataset_loader::PlagiarismDataset, evaluation::{accuracy_threshold_curve_for_config, average_error_for_config}, genetic_evolution::ConfigCodec, persistence::{MetricsEventHandler, PersistingCheckpointer}, plagiarism_runner::{Config, PlagiarismOutput}
};
use radiate::prelude::*;

pub fn run_evolution(dataset: &'static PlagiarismDataset) {
    // Persistance / Checkpoints
    let checkpointer = PersistingCheckpointer::load("out/evolution/", dataset);
    let checkpointer = Arc::new(checkpointer);
    
    // Base setup
    let codec = ConfigCodec;
    let mut engine = GeneticEngine::builder()
        .minimizing()
        .codec(codec)
        .fitness_fn(move |config| average_error_for_config(&config, dataset));

    // Infrastructure
    engine = engine
        .subscribe(MetricsEventHandler::from_arc(&checkpointer))
        .executor(Executor::WorkerPool);

    // Run the engine
    let mut engine = engine.build();
    let result = engine.run(|epoch| {
            checkpointer.persist_checkpoint(epoch.ecosystem(), epoch.index());
            epoch.score().as_f32() < 0.05
        });

    println!("Best solution found: {:?}", result.value());
}
