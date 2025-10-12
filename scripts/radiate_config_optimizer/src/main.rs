#![feature(try_blocks)]

mod dataset_loader;
mod plagiarism_runner;
mod genetic_evolution;
mod evaluation;
mod evolution_runner;
mod persistence;

use std::path::Path;

use dataset_loader::load_dataset;
use fxhash::{FxHashMap, FxHashSet};
use itertools::Itertools;
use plagiarism_runner::PlagiarismRunner;
use radiate::{EngineExt, Executor, GeneticEngine};

use crate::{dataset_loader::PlagiarismDataset, genetic_evolution::ConfigCodec, plagiarism_runner::{Config, PlagiarismOutput}};

fn main() {
    let dataset_root = Path::new("datasets/c_cpp_plagiarism");
    let dataset = load_dataset(dataset_root).unwrap();
    let global_dataset: &PlagiarismDataset = Box::leak(Box::new(dataset));

    evolution_runner::run_evolution(&global_dataset);
}
