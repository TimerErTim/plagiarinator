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

#[derive(Debug, Error)]
pub enum CppParseError {
    #[error("failed to set C++ language")]
    Language,
    #[error("failed to parse C++ code")]
    Parse,
}

pub fn try_parse_cpp_to_tree(source: &[u8]) -> Result<tree_sitter::Tree, CppParseError> {
    let mut parser = tree_sitter::Parser::new();
    parser
        .set_language(&tree_sitter_cpp::LANGUAGE.into())
        .map_err(|_| CppParseError::Language)?;

    parser
        .parse(source, None)
        .ok_or(CppParseError::Parse)
}

pub fn parse_cpp_to_tree(reader: impl Read) -> tree_sitter::Tree {
    let bytes = reader
        .bytes()
        .map(|b| b.expect("Failed to read bytes"))
        .collect::<Vec<u8>>();
    try_parse_cpp_to_tree(&bytes).expect("Failed to parse C++ code")
}

pub fn parsed_file_from_content(
    path: impl Into<PathBuf>,
    content: String,
) -> Result<ParsedFile, CppParseError> {
    let ast = try_parse_cpp_to_tree(content.as_bytes())?;
    Ok(ParsedFile {
        file_path: path.into(),
        file_content: content,
        ast: FlattenedAST::from_treesitter_ast(ast),
    })
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

pub fn similarity_score<B: Backend>(
    file_1: ParsedFile,
    file_2: ParsedFile,
    model: PlagiarismDecider<B>,
) -> Result<f64, PlagiarismAnalysisError> {
    let model = model.no_grad();
    let model_device = model.devices().first().unwrap().clone();

    let graph_1 = Graph::from_flattened_ast(&file_1.ast, &model_device)
        .ok_or(PlagiarismAnalysisError::EmptyAST(file_1.file_path.clone()))?;
    let graph_2 = Graph::from_flattened_ast(&file_2.ast, &model_device)
        .ok_or(PlagiarismAnalysisError::EmptyAST(file_2.file_path.clone()))?;

    let score = model.forward(graph_1, graph_2);
    Ok(score.into_scalar().to_f64())
}

pub fn file_embedding<B: Backend>(
    file: ParsedFile,
    model: PlagiarismDecider<B>,
) -> Result<Tensor<B, 1>, PlagiarismAnalysisError> {
    let model = model.no_grad();
    let model_device = model.devices().first().unwrap().clone();

    let graph = Graph::from_flattened_ast(&file.ast, &model_device)
        .ok_or(PlagiarismAnalysisError::EmptyAST(file.file_path))?;
    let embedded_graph = model.embed_graph(graph);
    Ok(model.compress_embedded_graph(embedded_graph))
}

pub fn similarity_from_embeddings<B: Backend>(
    embedding_1: Tensor<B, 1>,
    embedding_2: Tensor<B, 1>,
    model: PlagiarismDecider<B>,
) -> f64 {
    let model = model.no_grad();
    model
        .similarity_probability(embedding_1, embedding_2)
        .into_scalar()
        .to_f64()
}

pub fn analyze_plagiarism<B: AutodiffBackend>(
    file_1: ParsedFile,
    file_2: ParsedFile,
    model: PlagiarismDecider<B>,
    normalize_importance: bool,
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
            importance: leaf_nodes_hirarchical_mean_importance(
                &file_1.ast,
                nodes_1_importance
                    .iter_dim(0)
                    .map(|weight_t| weight_t.into_scalar().to_f64()),
                normalize_importance,
            ),
        },
        file_2: AnalyzedFile {
            file_path: file_2.file_path.clone(),
            file_content: file_2.file_content.clone(),
            importance: leaf_nodes_hirarchical_mean_importance(
                &file_2.ast,
                nodes_2_importance
                    .iter_dim(0)
                    .map(|weight_t| weight_t.into_scalar().to_f64()),
                normalize_importance,
            ),
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
    normalize: bool,
) -> Vec<(f64, FileRange)> {
    let graph_weights: Vec<f64> = node_weights.collect();
    let expected_graph_nodes = ast.nodes.len().saturating_sub(1);
    if graph_weights.len() != expected_graph_nodes || expected_graph_nodes == 0 {
        return vec![];
    }

    // Graph construction drops the AST root (index 0); graph node i maps to AST node i + 1.
    let mut ast_weights = vec![0.0; ast.nodes.len()];
    for (graph_idx, &weight) in graph_weights.iter().enumerate() {
        ast_weights[graph_idx + 1] = weight;
    }

    let parents_map: FxHashMap<usize, usize> = ast
        .edges
        .iter()
        .map(|(child, parent)| (*child, *parent))
        .collect();
    let nodes_with_children: FxHashSet<usize> = parents_map.values().copied().collect();

    let mut leaf_weights = Vec::new();
    for (node_idx, node) in ast.nodes.iter().enumerate().skip(1) {
        if nodes_with_children.contains(&node_idx) {
            continue;
        }
        let importance =
            hierarchical_path_mean_importance(node_idx, &ast_weights, &parents_map);
        leaf_weights.push((importance, node.range.clone()));
    }

    if leaf_weights.is_empty() {
        return vec![];
    }

    if normalize {
        normalize_importance_weights(&mut leaf_weights);
    }

    leaf_weights
}

fn hierarchical_path_mean_importance(
    leaf_idx: usize,
    ast_weights: &[f64],
    parents_map: &FxHashMap<usize, usize>,
) -> f64 {
    let mut sum = 0.0;
    let mut count = 0;
    let mut current = leaf_idx;

    while current != 0 {
        sum += ast_weights[current];
        count += 1;
        current = parents_map.get(&current).copied().unwrap_or(0);
    }

    if count == 0 {
        return 0.0;
    }

    sum / count as f64
}

fn normalize_importance_weights(leaf_weights: &mut [(f64, FileRange)]) {
    let (min, max) = leaf_weights
        .iter()
        .fold((f64::INFINITY, f64::NEG_INFINITY), |(min, max), (weight, _)| {
            (min.min(*weight), max.max(*weight))
        });

    if (max - min).abs() < f64::EPSILON {
        for (weight, _) in leaf_weights.iter_mut() {
            *weight = 0.5;
        }
        return;
    }

    for (weight, _) in leaf_weights.iter_mut() {
        *weight = (*weight - min) / (max - min);
    }
}
