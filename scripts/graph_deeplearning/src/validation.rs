use burn::{Tensor, module::Module, nn::loss::BinaryCrossEntropyLossConfig, tensor::{backend::Backend, cast::ToElement}};
use decider_model::PlagiarismDecider;
use serde::{Deserialize, Serialize};
use crate::loading::PlagiarismTrainItem;

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ClassificationStatistics {
    pub threshold: f64,
    pub true_positive: usize,
    pub true_negative: usize,
    pub false_positive: usize,
    pub false_negative: usize,
    pub precision: f64,
    pub recall: f64,
    pub f1_score: f64,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ValidationPrediction {
    pub target: f32,
    pub prediction: f32,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ValidationResult {
    pub average_loss: f64,
    pub item_count: usize,
    pub raw_predictions: Vec<ValidationPrediction>,
    pub classification_statistics: Vec<ClassificationStatistics>,
    pub best_classification_statistic: ClassificationStatistics,
}

pub fn validate<B: Backend>(
    model: PlagiarismDecider<B>,
    test_dataset: impl Iterator<Item = PlagiarismTrainItem<B>>,
) -> ValidationResult {
    let device = model.devices().first().unwrap().clone();
    let loss_fn = BinaryCrossEntropyLossConfig::new().init(&device);

    let mut predictions = Vec::with_capacity(test_dataset.size_hint().0);
    let mut targets = Vec::with_capacity(test_dataset.size_hint().0);
    let mut targets_bool = Vec::with_capacity(test_dataset.size_hint().0);

    for item in test_dataset {
        let prediction = model.forward(item.graph_1.clone(), item.graph_2.clone());
        predictions.push(prediction.clone());
        targets.push(Tensor::from_ints([if item.label { 1 } else { 0 }], &device));
        targets_bool.push(item.label);
    }
    let predictions = Tensor::cat(predictions, 0);
    let targets = Tensor::cat(targets, 0);
    let loss = loss_fn
        .forward(predictions.clone(), targets.clone());

    validation_from_tensor_predictions(loss, predictions, targets.float())
}

pub fn validation_from_tensor_predictions<B: Backend>(
    loss: Tensor<B, 1>,
    predictions: Tensor<B, 1>,
    targets: Tensor<B, 1>,
) -> ValidationResult {
    let loss = loss.into_scalar().to_f64();
    let predictions = predictions_from_tensors(predictions, targets);
    let classification_statistics = all_classification_statistics_from_predictions(&predictions);
    let best_classification_statistic = classification_statistics.iter().max_by(|s1, s2| s1.f1_score.partial_cmp(&s2.f1_score).unwrap()).unwrap().clone();
    ValidationResult {
        average_loss: loss,
        item_count: predictions.len(),
        raw_predictions: predictions,
        classification_statistics,
        best_classification_statistic,
    }
}

pub fn predictions_from_tensors<B: Backend>(
    predictions: Tensor<B, 1>,
    targets: Tensor<B, 1>,
) -> Vec<ValidationPrediction> {
    let mut validated_predictions = Vec::with_capacity(predictions.shape()[0]);
    for (prediction, target) in predictions.iter_dim(0).zip(targets.iter_dim(0)) {
        let prediction = prediction.into_scalar().to_f32();
        let target = target.into_scalar().to_f32();
        validated_predictions.push(ValidationPrediction { prediction, target });
    }
    validated_predictions
}

pub fn classification_statistics_from_predictions(
    predictions: &[ValidationPrediction],
    threshold: f64,
) -> ClassificationStatistics {
    let mut true_positive = 0;
    let mut true_negative = 0;
    let mut false_positive = 0;
    let mut false_negative = 0;

    for prediction in predictions {
        match (prediction.prediction > threshold as f32, prediction.target > 0.5) {
            (true, true) => true_positive += 1,
            (false, false) => true_negative += 1,
            (true, false) => false_positive += 1,
            (false, true) => false_negative += 1,
        }
    }

    let precision = if true_positive + false_positive > 0 {
        true_positive as f64 / (true_positive + false_positive) as f64
    } else {
        0.0
    };
    let recall = if true_positive + false_negative > 0 {
        true_positive as f64 / (true_positive + false_negative) as f64
    } else {
        0.0
    };
    let f1_score = if precision + recall > 0.0 {
        2.0 * precision * recall / (precision + recall)
    } else {
        0.0
    };
    ClassificationStatistics {
        threshold,
        true_positive,
        true_negative,
        false_positive,
        false_negative,
        precision,
        recall,
        f1_score,
    }
}

pub fn all_classification_statistics_from_predictions(
    predictions: &[ValidationPrediction],
) -> Vec<ClassificationStatistics> {
    let steps = 100;
    (0..=steps).map(|i| {
        let threshold = i as f64 / steps as f64;
        classification_statistics_from_predictions(&predictions, threshold)
    }).collect()
}
