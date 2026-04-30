mod ast;
mod graph;

use std::path::PathBuf;

pub use ast::*;
use burn::{
    module::Module,
    prelude::Backend,
    tensor::{activation::softmax, backend::AutodiffBackend, Int},
};
pub use graph::*;
use thiserror::Error;

use crate::nn::PlagiarismDecider;

#[derive(Debug, Clone)]
pub struct PlagiarismTrainItem<B: Backend> {
    pub graph_1: Graph<B, Int>,
    pub graph_2: Graph<B, Int>,
    /// True if plagiarization, false otherwise
    pub label: bool,
}

pub struct PlagiarismInsights {
    pub file_1: AnalyzedFile,
    pub file_2: AnalyzedFile,
    pub plagiarism_score: f64,
}

#[derive(Debug, Error)]
pub enum PlagiarismAnalysisError {
    #[error("AST is empty")]
    EmptyAST,
}

pub struct AnalyzedFile {
    pub file_path: PathBuf,
    pub file_content: String,
    /// Weight (contribution to the plagiarism) and ASTNode
    pub ast_nodes: Vec<(f64, ASTNode)>,
}

pub struct ParsedFile {
    pub file_path: PathBuf,
    pub file_content: String,
    pub ast: FlattenedAST,
}

pub fn analyze_plagiarism<B: AutodiffBackend>(
    file_1: ParsedFile,
    file_2: ParsedFile,
    model: PlagiarismDecider<B>,
) -> Result<PlagiarismInsights, PlagiarismAnalysisError>
where
    B::FloatElem: Into<f64>,
{
    // Mark model as not requiring gradients
    let model = model.no_grad();
    let model_devices = model.devices();
    let model_device = model_devices.first().unwrap();

    // For both files, build a graph
    let graph_1 = Graph::from_flattened_ast(&file_1.ast, model_device)
        .ok_or(PlagiarismAnalysisError::EmptyAST)?;
    let graph_2 = Graph::from_flattened_ast(&file_2.ast, model_device)
        .ok_or(PlagiarismAnalysisError::EmptyAST)?;

    // For both graphs, embed the nodes and mark them as requiring gradients
    let embedded_nodes_1 = model.embed_nodes(graph_1.nodes).require_grad();
    let embedded_nodes_2 = model.embed_nodes(graph_2.nodes).require_grad();
    let embedded_graph_1 = Graph::new(embedded_nodes_1.clone(), graph_1.edges);
    let embedded_graph_2 = Graph::new(embedded_nodes_2.clone(), graph_2.edges);

    // Perform forward pass in order to get the plagiarism likelihood score
    let score = model.predict_embedded_graphs(embedded_graph_1, embedded_graph_2);
    let score_float = score.clone().into_scalar().into();

    // Perform backward pass in order to get the gradients/influence contribution of the embedded nodes
    let mut grads = score.backward();
    let node_1_importance = softmax(
        embedded_nodes_1
            .grad_remove(&mut grads)
            .unwrap()
            .abs()
            .sum_dim(1),
        0,
    );
    let node_2_importance = softmax(
        embedded_nodes_2
            .grad_remove(&mut grads)
            .unwrap()
            .abs()
            .sum_dim(1),
        0,
    );

    // Transform data into insights
    let insights = PlagiarismInsights {
        file_1: AnalyzedFile {
            file_path: file_1.file_path,
            file_content: file_1.file_content,
            ast_nodes: file_1
                .ast
                .nodes
                .into_iter()
                .zip(
                    node_1_importance
                        .iter_dim(0)
                        .map(|weight_t| weight_t.into_scalar().into()),
                )
                .map(|(node, weight)| (weight, node))
                .collect(),
        },
        file_2: AnalyzedFile {
            file_path: file_2.file_path,
            file_content: file_2.file_content,
            ast_nodes: file_2
                .ast
                .nodes
                .into_iter()
                .zip(
                    node_2_importance
                        .iter_dim(0)
                        .map(|weight_t| weight_t.into_scalar().into()),
                )
                .map(|(node, weight)| (weight, node))
                .collect(),
        },
        plagiarism_score: score_float,
    };
    Ok(insights)
}
