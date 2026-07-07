mod annembed_pipeline;

use std::path::{Path, PathBuf};

use burn::{
    module::Module,
    tensor::{backend::Backend, cast::ToElement, Tensor},
};
use clap::Parser;
use data_loading::train_test_cpp_dataset;
use decider_model::data::{embed_files, ParsedFile};
use graph_deeplearning::{load_trained_model, loading::make_testset_loader};
use graph_deeplearning::loading::PlagiarismTestItem;
use rustc_hash::FxHashMap;
use serde::Serialize;

#[derive(Parser)]
#[command(about = "Export validation file embeddings reduced to 2D via annembed")]
struct Args {
    /// Override output path (default: out/annembed/{MODEL_WEIGHTS_PATH stem}.json)
    #[arg(long)]
    output: Option<PathBuf>,
}

#[derive(Serialize)]
struct EmbeddingExport {
    component1: f64,
    component2: f64,
    self_path: String,
    plag_paths: Vec<String>,
}

struct ValidationFiles {
    files: Vec<ParsedFile>,
    plag_paths: Vec<Vec<PathBuf>>,
}

fn canonical_path(path: &Path) -> String {
    path.canonicalize()
        .unwrap_or_else(|_| path.to_path_buf())
        .to_string_lossy()
        .into_owned()
}

fn collect_validation_files(test_items: impl Iterator<Item = PlagiarismTestItem>) -> ValidationFiles {
    let mut files = Vec::new();
    let mut path_to_index = FxHashMap::<String, usize>::default();
    let mut plag_paths: Vec<Vec<PathBuf>> = Vec::new();

    for item in test_items {
        let idx_1 = *path_to_index
            .entry(item.file_1.file_path.to_string_lossy().to_string())
            .or_insert_with(|| {
                files.push(item.file_1);
                plag_paths.push(Vec::new());
                files.len() - 1
            });
        let idx_2 = *path_to_index
            .entry(item.file_2.file_path.to_string_lossy().to_string())
            .or_insert_with(|| {
                files.push(item.file_2);
                plag_paths.push(Vec::new());
                files.len() - 1
            });

        if item.label {
            plag_paths[idx_1].push(files[idx_2].file_path.clone());
            plag_paths[idx_2].push(files[idx_1].file_path.clone());
        }
    }

    for paths in &mut plag_paths {
        paths.sort();
        paths.dedup();
    }

    ValidationFiles { files, plag_paths }
}

fn representation_to_vec<B: Backend>(tensor: Tensor<B, 1>) -> Vec<f32> {
    tensor
        .iter_dim(0)
        .map(|value| value.into_scalar().to_f32())
        .collect()
}

fn default_output_path() -> PathBuf {
    let weights_path = std::env::var("MODEL_WEIGHTS_PATH")
        .expect("MODEL_WEIGHTS_PATH environment variable must be set");
    let stem = Path::new(&weights_path)
        .file_stem()
        .expect("MODEL_WEIGHTS_PATH must have a filename")
        .to_string_lossy();
    PathBuf::from(format!("out/annembed/{stem}.json"))
}

fn main() {
    let args = Args::parse();
    type InferenceBackend = burn::backend::Flex;
    let device = burn::backend::flex::FlexDevice::default();

    let (_, cpp_test_dataset) = train_test_cpp_dataset().expect("failed to load test dataset");
    let test_loader = make_testset_loader(
        cpp_test_dataset.plagiarized_pairs,
        cpp_test_dataset.authentic_pairs,
    );

    let validation = collect_validation_files(test_loader.iter());
    eprintln!(
        "Collected {} unique validation files",
        validation.files.len()
    );

    let model = load_trained_model::<InferenceBackend>(&device);
    let model = model.no_grad();
    let embeddings = embed_files(&model, &validation.files).expect("failed to embed files");

    let vectors: Vec<Vec<f32>> = embeddings
        .into_iter()
        .map(representation_to_vec)
        .collect();

    eprintln!("Running annembed dimensionality reduction...");
    let reduced = annembed_pipeline::reduce_dimensions(&vectors, 2);

    let export: Vec<EmbeddingExport> = validation
        .files
        .iter()
        .zip(validation.plag_paths.iter())
        .zip(reduced.iter())
        .map(|((file, plag_paths), coords)| EmbeddingExport {
            component1: coords[0],
            component2: coords[1],
            self_path: canonical_path(&file.file_path),
            plag_paths: plag_paths.iter().map(|p| canonical_path(p)).collect(),
        })
        .collect();

    let output_path = args.output.unwrap_or_else(default_output_path);
    if let Some(parent) = output_path.parent() {
        std::fs::create_dir_all(parent).expect("failed to create output directory");
    }

    let file = std::fs::File::create(&output_path).expect("failed to create output file");
    serde_json::to_writer_pretty(file, &export).expect("failed to write JSON export");

    eprintln!("Wrote {} entries to {}", export.len(), output_path.display());
}
