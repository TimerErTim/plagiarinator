use std::sync::OnceLock;

use burn::backend::{autodiff::Autodiff, Flex};
use burn::store::BurnpackStore;
use burn::tensor::cast::ToElement;
use burn::Tensor;
use decider_model::{
    data::{
        analyze_plagiarism, file_embedding, parsed_file_from_content, similarity_from_embeddings,
        similarity_score, AnalyzedFile, CppParseError, PlagiarismAnalysisError,
    },
    load_model, PlagiarismDecider, REPRESENTATION_SIZE,
};
use serde::Serialize;
use wasm_bindgen::prelude::*;

type InferenceBackend = Flex;
type InferenceModel = PlagiarismDecider<InferenceBackend>;
type AutodiffModel = PlagiarismDecider<Autodiff<InferenceBackend>>;

static INFERENCE_MODEL: OnceLock<InferenceModel> = OnceLock::new();
static AUTODIFF_MODEL: OnceLock<AutodiffModel> = OnceLock::new();

const MODEL_WEIGHTS: &[u8] = include_bytes!("../weights/model.bpk");

fn plagiarism_threshold() -> f64 {
    option_env!("PLAGIARISM_THRESHOLD")
        .and_then(|value| value.parse().ok())
        .unwrap_or(0.5)
}

fn inference_model() -> &'static InferenceModel {
    INFERENCE_MODEL.get_or_init(|| {
        let device = Default::default();
        let mut store = BurnpackStore::from_static(MODEL_WEIGHTS);
        load_model(&device, &mut store)
    })
}

fn autodiff_model() -> &'static AutodiffModel {
    AUTODIFF_MODEL.get_or_init(|| {
        let device = Default::default();
        let mut store = BurnpackStore::from_static(MODEL_WEIGHTS);
        load_model(&device, &mut store)
    })
}

fn representation_to_vec(tensor: Tensor<InferenceBackend, 1>) -> Vec<f32> {
    tensor
        .iter_dim(0)
        .map(|value| value.into_scalar().to_f32())
        .collect()
}

fn tensor_from_embedding(embedding: &[f32]) -> Result<Tensor<InferenceBackend, 1>, String> {
    if embedding.len() != REPRESENTATION_SIZE {
        return Err(format!(
            "expected embedding length {REPRESENTATION_SIZE}, got {}",
            embedding.len()
        ));
    }
    let device = Default::default();
    Ok(Tensor::from_data(embedding, &device))
}

#[derive(Serialize)]
struct CompareResponse {
    score: f64,
    decision: &'static str,
    threshold: f64,
}

#[derive(Serialize)]
struct EmbedResponse {
    embedding: Vec<f32>,
}

#[derive(Serialize)]
struct ErrorResponse {
    error: String,
}

#[derive(Serialize)]
struct ImportanceSpan {
    weight: f64,
    start_byte: usize,
    end_byte: usize,
    start_line: usize,
    start_column: usize,
    end_line: usize,
    end_column: usize,
}

#[derive(Serialize)]
struct AnalyzedFileResponse {
    importance: Vec<ImportanceSpan>,
}

#[derive(Serialize)]
struct AnalyzeResponse {
    score: f64,
    decision: &'static str,
    threshold: f64,
    file_1: AnalyzedFileResponse,
    file_2: AnalyzedFileResponse,
}

fn decision_for(score: f64) -> &'static str {
    if score >= plagiarism_threshold() {
        "plagiarism"
    } else {
        "original"
    }
}

fn parse_error_message(error: &CppParseError) -> String {
    error.to_string()
}

fn analysis_error_message(error: &PlagiarismAnalysisError) -> String {
    error.to_string()
}

fn analyzed_file_to_response(file: AnalyzedFile) -> AnalyzedFileResponse {
    AnalyzedFileResponse {
        importance: file
            .importance
            .into_iter()
            .map(|(weight, range)| ImportanceSpan {
                weight,
                start_byte: range.start_byte,
                end_byte: range.end_byte,
                start_line: range.start_line,
                start_column: range.start_column,
                end_line: range.end_line,
                end_column: range.end_column,
            })
            .collect(),
    }
}

#[wasm_bindgen(start)]
pub fn init_wasm() {
    console_error_panic_hook::set_once();
    let _ = inference_model();
    let _ = autodiff_model();
}

#[wasm_bindgen]
pub fn plagiarism_threshold_value() -> f64 {
    plagiarism_threshold()
}

#[wasm_bindgen]
pub fn representation_size_value() -> usize {
    REPRESENTATION_SIZE
}

#[wasm_bindgen]
pub fn embed_source(content: &str) -> JsValue {
    let parsed = match parsed_file_from_content("source.cpp", content.to_string()) {
        Ok(file) => file,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse {
                error: parse_error_message(&error),
            })
            .unwrap_or(JsValue::NULL);
        }
    };

    match file_embedding(parsed, inference_model().clone()) {
        Ok(embedding) => serde_wasm_bindgen::to_value(&EmbedResponse {
            embedding: representation_to_vec(embedding),
        })
        .unwrap_or(JsValue::NULL),
        Err(error) => serde_wasm_bindgen::to_value(&ErrorResponse {
            error: analysis_error_message(&error),
        })
        .unwrap_or(JsValue::NULL),
    }
}

#[wasm_bindgen]
pub fn compare_embeddings(embedding_1: &[f32], embedding_2: &[f32]) -> JsValue {
    let tensor_1 = match tensor_from_embedding(embedding_1) {
        Ok(tensor) => tensor,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse { error })
                .unwrap_or(JsValue::NULL);
        }
    };
    let tensor_2 = match tensor_from_embedding(embedding_2) {
        Ok(tensor) => tensor,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse { error })
                .unwrap_or(JsValue::NULL);
        }
    };

    let score = similarity_from_embeddings(tensor_1, tensor_2, inference_model().clone());
    serde_wasm_bindgen::to_value(&CompareResponse {
        score,
        decision: decision_for(score),
        threshold: plagiarism_threshold(),
    })
    .unwrap_or(JsValue::NULL)
}

#[wasm_bindgen]
pub fn compare_sources(file_1: &str, file_2: &str) -> JsValue {
    let parsed_1 = match parsed_file_from_content("file_1.cpp", file_1.to_string()) {
        Ok(file) => file,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse {
                error: parse_error_message(&error),
            })
            .unwrap_or(JsValue::NULL);
        }
    };
    let parsed_2 = match parsed_file_from_content("file_2.cpp", file_2.to_string()) {
        Ok(file) => file,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse {
                error: parse_error_message(&error),
            })
            .unwrap_or(JsValue::NULL);
        }
    };

    match similarity_score(parsed_1, parsed_2, inference_model().clone()) {
        Ok(score) => serde_wasm_bindgen::to_value(&CompareResponse {
            score,
            decision: decision_for(score),
            threshold: plagiarism_threshold(),
        })
        .unwrap_or(JsValue::NULL),
        Err(error) => serde_wasm_bindgen::to_value(&ErrorResponse {
            error: analysis_error_message(&error),
        })
        .unwrap_or(JsValue::NULL),
    }
}

#[wasm_bindgen]
pub fn analyze_importance(file_1: &str, file_2: &str) -> JsValue {
    let parsed_1 = match parsed_file_from_content("file_1.cpp", file_1.to_string()) {
        Ok(file) => file,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse {
                error: parse_error_message(&error),
            })
            .unwrap_or(JsValue::NULL);
        }
    };
    let parsed_2 = match parsed_file_from_content("file_2.cpp", file_2.to_string()) {
        Ok(file) => file,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse {
                error: parse_error_message(&error),
            })
            .unwrap_or(JsValue::NULL);
        }
    };

    let score = match similarity_score(
        parsed_1.clone(),
        parsed_2.clone(),
        inference_model().clone(),
    ) {
        Ok(score) => score,
        Err(error) => {
            return serde_wasm_bindgen::to_value(&ErrorResponse {
                error: analysis_error_message(&error),
            })
            .unwrap_or(JsValue::NULL);
        }
    };

    match analyze_plagiarism(parsed_1, parsed_2, autodiff_model().clone(), true) {
        Ok(insights) => serde_wasm_bindgen::to_value(&AnalyzeResponse {
            score,
            decision: decision_for(score),
            threshold: plagiarism_threshold(),
            file_1: analyzed_file_to_response(insights.file_1),
            file_2: analyzed_file_to_response(insights.file_2),
        })
        .unwrap_or(JsValue::NULL),
        Err(error) => serde_wasm_bindgen::to_value(&ErrorResponse {
            error: analysis_error_message(&error),
        })
        .unwrap_or(JsValue::NULL),
    }
}
