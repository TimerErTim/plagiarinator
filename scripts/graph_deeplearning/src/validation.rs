use crate::loading::{PlagiarismTestItem, PlagiarismTrainItem};
use burn::{
    backend::Flex,
    module::Module,
    nn::loss::BinaryCrossEntropyLossConfig,
    tensor::{backend::Backend, cast::ToElement},
    Tensor,
};
use decider_model::{data::analyze_plagiarism_pairs, PlagiarismDecider};
use rustc_hash::FxHashMap;
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ClassificationStatistics {
    pub threshold: f64,
    pub true_positive: usize,
    pub true_negative: usize,
    pub false_positive: usize,
    pub false_negative: usize,
    pub false_positive_rate: f64,
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
    /// Default threshold of 0.5 is unbiased
    pub unbiased_classification_statistic: ClassificationStatistics,
    pub roc_auc: f64,
    pub pr_auc: f64,
}

pub fn validate<B: Backend>(
    model: PlagiarismDecider<B>,
    test_dataset: impl Iterator<Item = PlagiarismTestItem>,
) -> ValidationResult {
    // Parse the items and pairs from the dataset based on file_paths
    let mut items = Vec::with_capacity(test_dataset.size_hint().0);
    let mut targets = Vec::with_capacity(test_dataset.size_hint().0);
    let mut item_indices = FxHashMap::<String, usize>::default();
    let mut indices_pairs = Vec::with_capacity(test_dataset.size_hint().0);
    for item in test_dataset {
        let item_1_index = item_indices
            .entry(item.file_1.file_path.to_string_lossy().to_string())
            .or_insert_with(|| {
                items.push(item.file_1);
                items.len() - 1
            }).clone();
        let item_2_index = item_indices
            .entry(item.file_2.file_path.to_string_lossy().to_string())
            .or_insert_with(|| {
                items.push(item.file_2);
                items.len() - 1
            }).clone();
        indices_pairs.push((item_1_index, item_2_index));
        targets.push(item.label);
    }

    // Let analyze_plagiarism do the heavy lifting
    let analyzed_output = analyze_plagiarism_pairs(&items, &indices_pairs, model.clone()).unwrap();

    validation_from_predictions(
        analyzed_output
            .pairs
            .into_iter()
            .zip(targets)
            .map(|(pair, target)| ValidationPrediction {
                prediction: pair.plagiarism_score as f32,
                target: if target { 1.0 } else { 0.0 },
            })
            .collect(),
        None,
    )
}

pub fn validation_from_tensors<B: Backend>(
    loss: Tensor<B, 1>,
    predictions: Tensor<B, 1>,
    targets: Tensor<B, 1>,
) -> ValidationResult {
    let loss = loss.into_scalar().to_f64();
    let predictions = predictions_from_tensors(predictions, targets);

    validation_from_predictions(predictions, Some(loss))
}

pub fn validation_from_predictions(
    predictions: Vec<ValidationPrediction>,
    // Is calculated from the predictions if not provided
    loss: Option<f64>,
) -> ValidationResult {
    // Calculate loss if not provided
    let loss = loss.unwrap_or_else(|| {
        let device = burn::backend::flex::FlexDevice::default();
        let loss = BinaryCrossEntropyLossConfig::new().init::<Flex>(&device);
        let targets = Tensor::from_data(
            predictions
                .iter()
                .map(|p| if p.target >= 0.5 { 1 } else { 0 })
                .collect::<Vec<_>>()
                .as_slice(),
            &device,
        );
        let predictions = Tensor::from_data(
            predictions
                .iter()
                .map(|p| p.prediction)
                .collect::<Vec<_>>()
                .as_slice(),
            &device,
        );
        loss.forward::<1>(predictions, targets)
            .into_scalar()
            .to_f64()
    });

    let mut classification_statistics =
        all_classification_statistics_from_predictions(&predictions);
    let best_classification_statistic = classification_statistics
        .iter()
        .max_by(|s1, s2| s1.f1_score.partial_cmp(&s2.f1_score).unwrap())
        .unwrap()
        .clone();
    classification_statistics.sort_by(|s1, s2| s1.threshold.partial_cmp(&s2.threshold).unwrap());
    let unbiased_classification_statistic =
        classification_statistics[classification_statistics.len() / 2].clone();

    let roc_auc = area_under_curve(
        classification_statistics
            .iter()
            .map(|s| (s.false_positive_rate, s.recall))
            .collect(),
    );
    let pr_auc = area_under_curve(
        classification_statistics
            .iter()
            .map(|s| (s.recall, s.precision))
            .collect(),
    );

    ValidationResult {
        average_loss: loss,
        item_count: predictions.len(),
        raw_predictions: predictions,
        classification_statistics,
        best_classification_statistic,
        unbiased_classification_statistic,
        roc_auc,
        pr_auc,
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
        match (
            prediction.prediction.clamp(0.0, 1.0) >= threshold as f32,
            prediction.target >= 0.5,
        ) {
            (true, true) => true_positive += 1,
            (false, false) => true_negative += 1,
            (true, false) => false_positive += 1,
            (false, true) => false_negative += 1,
        }
    }

    // We expect/require at least one negative class in the test set
    let false_positive_rate = false_positive as f64 / (true_negative + false_positive) as f64;

    let precision_lenient = if true_positive + false_positive > 0 {
        true_positive as f64 / (true_positive + false_positive) as f64
    } else {
        1.0 // If no positive prediction, let's pretend we have perfect precision (no wrongs)
    };
    let precision_strict = if true_positive + false_positive > 0 {
        precision_lenient
    } else {
        0.0
    };

    // We expect/require at least one positive class in the test set
    let recall = true_positive as f64 / (true_positive + false_negative) as f64;

    let f1_score = if precision_strict + recall > 0.0 {
        2.0 * precision_strict * recall / (precision_strict + recall)
    } else {
        0.0
    };
    ClassificationStatistics {
        threshold,
        true_positive,
        true_negative,
        false_positive,
        false_negative,
        false_positive_rate,
        precision: precision_lenient,
        recall,
        f1_score,
    }
}

pub fn all_classification_statistics_from_predictions(
    predictions: &[ValidationPrediction],
) -> Vec<ClassificationStatistics> {
    let steps = 250;
    (0..=steps)
        .map(|i| {
            let threshold = i as f64 / steps as f64;
            classification_statistics_from_predictions(predictions, threshold)
        })
        .collect()
}

pub fn area_under_curve(mut points: Vec<(f64, f64)>) -> f64 {
    // Sort points by x coordinate, then by y coordinate for ties
    points.sort_by(|a, b| {
        a.0.partial_cmp(&b.0)
            .unwrap_or(std::cmp::Ordering::Equal)
            .then(a.1.partial_cmp(&b.1).unwrap_or(std::cmp::Ordering::Equal))
    });

    let mut area = 0.0;
    for i in 0..(points.len() - 1) {
        let (x0, y0) = points[i];
        let (x1, y1) = points[i + 1];
        let (dx, _dy) = (x1 - x0, y1 - y0);
        if dx > 0.0 {
            let segment_area = dx * (y0 + y1) / 2.0;
            area += if segment_area > 0.0 {
                segment_area
            } else {
                0.0
            };
        }
    }
    area
}
