use std::{
    io::Read,
    path::{Path, PathBuf},
};

use burn::{
    module::Module,
    tensor::{backend::AutodiffBackend, cast::ToElement},
};
use rustc_hash::{FxHashMap, FxHashSet};
use serde::{Deserialize, Serialize};
use thiserror::Error;

use crate::{
    PlagiarismDecider,
    data::{FileRange, FlattenedAST, Graph},
};

pub fn parse_cpp_to_tree(reader: impl Read) -> tree_sitter::Tree {
    let mut parser = tree_sitter::Parser::new();
    parser
        .set_language(&tree_sitter_cpp::LANGUAGE.into())
        .expect("Failed to set language to C++");

    parser
        .parse(
            reader
                .bytes()
                .map(|b| b.expect("Failed to read bytes"))
                .collect::<Vec<u8>>()
                .as_slice(),
            None,
        )
        .expect("Failed to parse C++ code")
}

pub fn parse_cpp_file(path: impl AsRef<Path>) -> ParsedFile {
    let path = path.as_ref();
    let file_content = std::fs::read_to_string(path).expect("Failed to read file");
    let ast = parse_cpp_to_tree(file_content.as_bytes());
    ParsedFile {
        file_path: path.to_path_buf(),
        file_content,
        ast: FlattenedAST::from_treesitter_ast(ast),
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct PlagiarismInsights {
    pub plagiarism_score: f64,
    pub file_1: AnalyzedFile,
    pub file_2: AnalyzedFile,
}

#[derive(Debug, Error)]
pub enum PlagiarismAnalysisError {
    #[error("AST is empty")]
    EmptyAST,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct AnalyzedFile {
    pub file_path: PathBuf,
    pub file_content: String,
    /// Weight (contribution to the plagiarism) and FileRange
    pub importance: Vec<(f64, FileRange)>,
}

#[derive(Debug, Clone)]
pub struct ParsedFile {
    pub file_path: PathBuf,
    pub file_content: String,
    pub ast: FlattenedAST,
}

pub fn analyze_plagiarism<B: AutodiffBackend>(
    file_1: ParsedFile,
    file_2: ParsedFile,
    model: PlagiarismDecider<B>,
) -> Result<PlagiarismInsights, PlagiarismAnalysisError> {
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
    let score_float = score.clone().into_scalar().to_f64();

    // Perform backward pass in order to get the gradients/influence contribution of the embedded nodes
    let mut grads = score.backward();
    let node_1_importance = embedded_nodes_1.grad_remove(&mut grads).unwrap().sum_dim(1);
    let node_2_importance = embedded_nodes_2.grad_remove(&mut grads).unwrap().sum_dim(1);
    // Scale importance
    let highest_value = node_1_importance
        .clone()
        .max_abs()
        .max_pair(node_2_importance.clone().max_abs())
        .unsqueeze();
    let node_1_importance = node_1_importance.div(highest_value.clone());
    let node_2_importance = node_2_importance.div(highest_value);

    // Transform data into insights
    fn leaf_nodes_hirarchical_mean_importance(
        ast: FlattenedAST,
        node_weights: impl Iterator<Item = f64>,
    ) -> Vec<(f64, FileRange)> {
        // Skip the root node because it is not part of the graph representation
        let ast_nodes_importance = ast
            .nodes
            .into_iter()
            .enumerate()
            .skip(1)
            .zip(node_weights)
            .map(|((node_idx, node), weight)| (weight, node_idx, node.range));
        let mut parent_acc_imp = FxHashMap::default();
        let parents_map = ast
            .edges
            .iter()
            .map(|(child, parent)| (*child, *parent))
            .collect::<FxHashMap<_, _>>();
        let node_idx_with_children = parents_map.values().cloned().collect::<FxHashSet<_>>();
        let mut leafs = FxHashSet::default();
        for (weight, node_idx, range) in ast_nodes_importance {
            let (parent_weight, parent_counts) = parents_map
                .get(&node_idx)
                .cloned()
                .and_then(|parent_idx| parent_acc_imp.get(&parent_idx).cloned())
                .unwrap_or((0.0, 0));
            parent_acc_imp.insert(node_idx, (parent_weight + weight, parent_counts + 1));
            if !node_idx_with_children.contains(&node_idx) {
                leafs.insert((node_idx, range));
            }
        }
        leafs
            .into_iter()
            .map(|(node_idx, range)| {
                let (parent_weight, parent_counts) =
                    parent_acc_imp.get(&node_idx).cloned().unwrap_or((0.0, 1));
                (parent_weight / parent_counts as f64, range)
            })
            .collect()
    }

    let insights = PlagiarismInsights {
        file_1: AnalyzedFile {
            file_path: file_1.file_path,
            file_content: file_1.file_content,
            importance: leaf_nodes_hirarchical_mean_importance(
                file_1.ast,
                node_1_importance
                    .iter_dim(0)
                    .map(|weight_t| weight_t.into_scalar().to_f64()),
            ),
        },
        file_2: AnalyzedFile {
            file_path: file_2.file_path,
            file_content: file_2.file_content,
            importance: leaf_nodes_hirarchical_mean_importance(
                file_2.ast,
                node_2_importance
                    .iter_dim(0)
                    .map(|weight_t| weight_t.into_scalar().to_f64()),
            ),
        },
        plagiarism_score: score_float,
    };
    Ok(insights)
}
