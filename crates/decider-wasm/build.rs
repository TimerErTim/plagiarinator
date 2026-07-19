use std::{env, path::PathBuf};

fn main() {
    let threshold = env::var("PLAGIARISM_THRESHOLD").unwrap_or_else(|_| "0.5".to_string());
    println!("cargo:rustc-env=PLAGIARISM_THRESHOLD={threshold}");

    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let weights_path = env::var("MODEL_WEIGHTS_PATH")
        .map(PathBuf::from)
        .unwrap_or_else(|_| manifest_dir.join("weights/model.bpk"));

    if !weights_path.is_file() {
        panic!(
            "model weights not found at {} — set MODEL_WEIGHTS_PATH or place weights at crates/decider-wasm/weights/model.bpk",
            weights_path.display()
        );
    }

    println!("cargo:rerun-if-changed={}", weights_path.display());
    println!("cargo:rerun-if-env-changed=MODEL_WEIGHTS_PATH");
    println!("cargo:rerun-if-env-changed=PLAGIARISM_THRESHOLD");
}
