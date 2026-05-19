use std::{env, path::PathBuf};

use chrono::{DateTime, Utc};
use tensorboard_logger::TensorBoardLogger;

pub fn create_tensorboard_logger(
    experiment_name: impl AsRef<str>,
    experiment_timestamp: impl AsRef<str>,
) -> TensorBoardLogger {
    let path = PathBuf::from(env::var("TENSORBOARD_LOG_DIR").unwrap_or_else(|_| "out/tensorboard-logs".to_string()));
    let path = path.join(format!("{}_{}", experiment_timestamp.as_ref(), experiment_name.as_ref()));
    let tb = TensorBoardLogger::new(path).unwrap();
    tb
}

