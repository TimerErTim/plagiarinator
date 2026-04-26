mod graph;

use burn::{prelude::Backend, tensor::Int};
pub use graph::*;

#[derive(Debug, Clone)]
pub struct PlagiarismTrainItem<B: Backend> {
    pub graph_1: Graph<B, Int>,
    pub graph_2: Graph<B, Int>,
    /// True if plagiarization, false otherwise
    pub label: bool,
}
