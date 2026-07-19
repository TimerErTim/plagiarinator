use std::sync::OnceLock;

use burn::backend::{autodiff::Autodiff, ndarray::NdArray};
use burn::store::BurnpackStore;
use decider_model::{
    data::{
        analyze_plagiarism, parsed_file_from_content, similarity_score, AnalyzedFile,
        CppParseError, PlagiarismAnalysisError,
    },
    load_model, PlagiarismDecider,
};
use serde::Serialize;
use wasm_bindgen::prelude::*;

type InferenceModel = PlagiarismDecider<NdArray>;
type AutodiffModel = PlagiarismDecider<Autodiff<NdArray>>;

static INFERENCE_MODEL: OnceLock<InferenceModel> = OnceLock::new();

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

fn fresh_autodiff_model() -> AutodiffModel {
    let device = Default::default();
    let mut store = BurnpackStore::from_static(MODEL_WEIGHTS);
    load_model(&device, &mut store)
}

#[derive(Serialize)]
struct CompareResponse {
    score: f64,
    decision: &'static str,
    threshold: f64,
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
}

#[wasm_bindgen]
pub fn plagiarism_threshold_value() -> f64 {
    plagiarism_threshold()
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

    match analyze_plagiarism(parsed_1, parsed_2, fresh_autodiff_model(), false) {
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
