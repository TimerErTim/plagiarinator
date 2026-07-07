use std::{
    io::Read,
    path::{Path, PathBuf},
};

use burn::{
    Tensor, module::Module, tensor::{backend::{AutodiffBackend, Backend}, cast::ToElement},
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

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct MultiplePlagiarismInsights {
    pub files: Vec<StaticFile>,
    pub pairs: Vec<MultiplePlagiarismPair>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct MultiplePlagiarismPair {
    pub file_1_idx: usize,
    pub file_2_idx: usize,
    pub plagiarism_score: f64,
}

#[derive(Debug, Error)]
pub enum PlagiarismAnalysisError {
    #[error("AST is empty for file {0}")]
    EmptyAST(PathBuf),
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct StaticFile {
    pub file_idx: usize,
    pub file_path: PathBuf,
    pub file_content: String,
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
    let model_device = model.devices().first().unwrap().clone();

    // For both files, build a graph
    let graph_1 = Graph::from_flattened_ast(&file_1.ast, &model_device)
        .ok_or(PlagiarismAnalysisError::EmptyAST(file_1.file_path.clone()))?;
    let graph_2 = Graph::from_flattened_ast(&file_2.ast, &model_device)
        .ok_or(PlagiarismAnalysisError::EmptyAST(file_2.file_path.clone()))?;

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
    let nodes_1_grads = embedded_nodes_1.grad_remove(&mut grads).unwrap();
    let nodes_2_grads = embedded_nodes_2.grad_remove(&mut grads).unwrap();

    // Find L2 (Euclidean) magnitude of the gradients for each node embedding
    let nodes_1_importance = nodes_1_grads.square().sum_dim(1).sqrt();
    let nodes_2_importance = nodes_2_grads.square().sum_dim(1).sqrt();

    // Transform the pair into an insights
    Ok(PlagiarismInsights {
        plagiarism_score: score_float,
        file_1: AnalyzedFile {
            file_path: file_1.file_path.clone(),
            file_content: file_1.file_content.clone(),
            importance: leaf_nodes_hirarchical_mean_importance(&file_1.ast, nodes_1_importance.iter_dim(0).map(|weight_t| weight_t.into_scalar().to_f64())),
        },
        file_2: AnalyzedFile {
            file_path: file_2.file_path.clone(),
            file_content: file_2.file_content.clone(),
            importance: leaf_nodes_hirarchical_mean_importance(&file_2.ast, nodes_2_importance.iter_dim(0).map(|weight_t| weight_t.into_scalar().to_f64())),
        },
    })
}

pub fn embed_files<B: Backend>(
    model: &PlagiarismDecider<B>,
    files: &[ParsedFile],
) -> Result<Vec<Tensor<B, 1>>, PlagiarismAnalysisError> {
    let model_device = model.devices().first().unwrap().clone();

    eprintln!("Embedding the files...");
    let embeddings = files
        .iter()
        .map(|file| {
            eprint!(".");
            let graph = Graph::from_flattened_ast(&file.ast, &model_device)
                .ok_or(PlagiarismAnalysisError::EmptyAST(file.file_path.clone()))?;
            let embedded_graph = model.embed_graph(graph);
            Ok(model.compress_embedded_graph(embedded_graph))
        })
        .collect::<Result<Vec<_>, _>>()?;
    eprintln!("");

    Ok(embeddings)
}

pub fn analyze_plagiarism_pairs<B: Backend>(
    files: &[ParsedFile],
    indices: &[(usize, usize)],
    model: PlagiarismDecider<B>
) -> Result<MultiplePlagiarismInsights, PlagiarismAnalysisError> {
    let model = model.no_grad();
    let file_embeddings = embed_files(&model, files)?;

    // Compute pairwise similarities
    eprintln!("Computing pairwise similarities...");
    let mut pairs = Vec::with_capacity(file_embeddings.len() * (file_embeddings.len() - 1) / 2);
    for (i, j) in indices.iter().cloned() {
        eprint!(".");
        let file_1 = &file_embeddings[i];
        let file_2 = &file_embeddings[j];

        // Perform forward pass in order to get the plagiarism likelihood score
        let plagiarism_probability = model.similarity_probability(
            file_1.clone(),
            file_2.clone(),
        );
        let plagiarism_probability_float = plagiarism_probability.clone().into_scalar().to_f64();

        pairs.push(MultiplePlagiarismPair {
            file_1_idx: i,
            file_2_idx: j,
            plagiarism_score: plagiarism_probability_float,
        });
    }
    eprintln!("");

    Ok(MultiplePlagiarismInsights {
        files: files.into_iter().enumerate().map(|(file_idx, file)| StaticFile {
            file_idx,
            file_path: file.file_path.clone(),
            file_content: file.file_content.clone(),
        }).collect(),
        pairs,
    })
}

pub fn analyze_plagiarism_all_pairs<B: Backend>(
    files: &[ParsedFile],
    model: PlagiarismDecider<B>
) -> Result<MultiplePlagiarismInsights, PlagiarismAnalysisError> {
    let mut indices = Vec::with_capacity(files.len() * (files.len() - 1) / 2);
    for i in 0..files.len() {
        for j in 0..i {
            indices.push((i, j));
        }
    }
    analyze_plagiarism_pairs(files, &indices, model)
}

// Transform data into insights
fn leaf_nodes_hirarchical_mean_importance(
    ast: &FlattenedAST,
    node_weights: impl Iterator<Item = f64>,
) -> Vec<(f64, FileRange)> {
    // Skip the root node because it is not part of the graph representation
    let ast_nodes_importance = ast
        .nodes
        .iter()
        .enumerate()
        .skip(1)
        .zip(node_weights)
        .map(|((node_idx, node), weight)| (weight, node_idx, node.range.clone() ));
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
    let leaf_weights = leafs
        .into_iter()
        .map(|(node_idx, range)| {
            let (parent_weight, parent_counts) =
                parent_acc_imp.get(&node_idx).cloned().unwrap_or((0.0, 1));
            (f64::abs(parent_weight / parent_counts as f64), range)
        })
        .collect::<Vec<_>>();
    // Standardize the leaf weights: subtract mean and divide by std deviation
    if leaf_weights.is_empty() {
        return vec![];
    }
    let (min, max) = leaf_weights.iter().fold((f64::INFINITY, f64::NEG_INFINITY), |(min, max), (w, _)| {
        (min.min(*w), max.max(*w))
    });
    if (max - min).abs() < f64::EPSILON {
        // If all weights are equal, return zeros for all
        leaf_weights.into_iter().map(|(_, range)| (0.0, range)).collect()
    } else {
        leaf_weights
            .into_iter()
            .map(|(w, range)| ((w - min) / (max - min), range))
            .collect()
    }
}
