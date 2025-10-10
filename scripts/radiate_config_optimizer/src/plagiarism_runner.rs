use std::process::Command;
use std::path::Path;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct MetricWeight {
    pub x_scale: f64,
    pub exponent: f64,
    pub weight: f64,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Config {
    pub pqgram_p: i32,
    pub pqgram_q: i32,
    pub wl_subtree_jaccard_rounds: i32,
    pub pqgram_weight: MetricWeight,
    pub ted_weight: MetricWeight,
    pub node_cosine_weight: MetricWeight,
    pub bigram_cosine_weight: MetricWeight,
    pub wl_subtree_jaccard_weight: MetricWeight,
}

#[derive(Debug, Clone, Deserialize)]
pub struct MetricResult {
    pub name: String,
    pub value: f64,
}

#[derive(Debug, Clone, Deserialize)]
pub struct SimilarityResult {
    pub combined: f64,
    pub results: Vec<MetricResult>,
}

#[derive(Debug, Clone, Deserialize)]
pub struct ComparisonError {
    pub message: String,
    #[serde(rename = "type")]
    pub error_type: String,
    pub traceback: String,
}

#[derive(Debug, Clone, Deserialize)]
pub struct ComparisonResult {
    pub success: bool,
    pub duration: Option<String>,
    pub left_file: String,
    pub right_file: String,
    pub similarity: Option<SimilarityResult>,
    pub error: Option<ComparisonError>,
}

#[derive(Debug, Clone, Deserialize)]
pub struct PlagiarismOutput {
    pub total_comparisons: usize,
    pub failed_comparisons: usize,
    pub results: Vec<ComparisonResult>,
    pub fatal_error: Option<ComparisonError>,
}

pub struct PlagiarismRunner {
    python_executable: String,
}

impl PlagiarismRunner {
    pub fn new() -> Self {
        Self {
            python_executable: "python".to_string(),
        }
    }

    pub fn with_python_executable(python_executable: String) -> Self {
        Self {
            python_executable,
        }
    }

    pub fn run_comparison(
        &self,
        config: Option<&Config>,
        file_pairs: &[(String, String)],
        lang: Option<&str>,
    ) -> Result<PlagiarismOutput, Box<dyn std::error::Error>> {
        // Create temporary config file
        let mut global_temp_config_file = None;
        if let Some(config) = config {
            let config_json = serde_json::to_string_pretty(config)?;
            let mut temp_file = tempfile::NamedTempFile::new()?;
            let config_path = temp_file.path();
            std::fs::write(&config_path, config_json)?;
            global_temp_config_file = Some(temp_file);
        }

        // Build command arguments
        let mut args = vec!["-m".to_string(), "plagiarinator.cli".to_string()];
        
        // Add file pairs
        for (left, right) in file_pairs {
            args.push(left.clone());
            args.push(right.clone());
        }

        // Add config argument
        if let Some(config_path) = &global_temp_config_file {
            args.push("--config".to_string());
            args.push(config_path.path().to_string_lossy().to_string());
        }

        // Add language argument if provided
        if let Some(language) = lang {
            args.push("--lang".to_string());
            args.push(language.to_string());
        }

        // Execute the command
        let output = Command::new(&self.python_executable)
            .args(&args)
            .output()?;

        // Parse the output
        if output.status.success() {
            let stdout = String::from_utf8(output.stdout)?;
            let result: PlagiarismOutput = serde_json::from_str(&stdout)?;
            Ok(result)
        } else {
            let stderr = String::from_utf8(output.stderr)?;
            if !stderr.is_empty() {
                // Try to parse error output as JSON
                if let Ok(error_result) = serde_json::from_str::<PlagiarismOutput>(&stderr) {
                    Ok(error_result)
                } else {
                    Err(format!("Command failed with stderr: {}", stderr).into())
                }
            } else {
                Err(format!("Command failed with exit code: {}", output.status).into())
            }
        }
    }
}

impl Default for PlagiarismRunner {
    fn default() -> Self {
        Self::new()
    }
}