use std::{
    fmt::{Display, Write},
    num::NonZeroUsize,
};

use burn::{
    Tensor, prelude::Backend, tensor::{BasicOps, Float, Int, Slice, TensorKind}
};
use fxhash::{FxHashMap, FxHashSet};
use tree_sitter::Tree;

use crate::model::FlattenedAST;

#[derive(Debug, Clone)]
pub struct Graph<B: Backend, D: TensorKind<B> = Float> {
    /// Shape: [num_nodes, num_features]
    pub nodes: Tensor<B, 2, D>,
    /// Shape: [num_nodes, num_nodes]
    /// Adjacency matrix
    pub edges: Tensor<B, 2, Float>,
}

impl<B: Backend, D: TensorKind<B>> Graph<B, D> {
    pub fn new(nodes: Tensor<B, 2, D>, edges: Tensor<B, 2, Float>) -> Self {
        Graph {
            nodes,
            edges,
        }
    }

    /// Makes sure that every edge/src->target pair is present at most once.
    pub fn normalized_adjacency_matrix(&self) -> Tensor<B, 2, Float>
    {

        let degree_matrix = self.edges.clone().sum_dim(0);
        let degree_matrix_sqrt = degree_matrix.sqrt();
        let normalized_adjacency_matrix = self.edges.clone().div(degree_matrix_sqrt + 1e-6);
        normalized_adjacency_matrix
    }

    /// Switches the source and target of the edges
    pub fn swap_direction(&mut self) {
        self.edges = self.edges.clone().swap_dims(0, 1);
    }
}

impl<B: Backend, D: TensorKind<B> + BasicOps<B>> Display for Graph<B, D> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(
            f,
            "Graph (Nodes: {}, Edges: {})",
            self.nodes.shape()[0],
            self.edges.clone().greater_elem(0.0).int().sum().into_scalar()
        )?;
        writeln!(f, "Nodes: {}", self.nodes)?;
        Ok(())
    }
}

impl<B: Backend> Graph<B, Int> {
    pub fn from_treesitter_ast(ast: Tree, device: &B::Device) -> Option<Self> {
        Self::from_flattened_ast(&FlattenedAST::from_treesitter_ast(ast), device)
    }

    pub fn from_flattened_ast(ast: &FlattenedAST, device: &B::Device) -> Option<Self> {
        let nodes_tensors = ast.nodes
            .iter()
            .map(|node| Tensor::<_, 1, Int>::from_ints([node.kind_id as i32 - 1], device))
            .collect::<Vec<_>>();
        if nodes_tensors.is_empty() {
            return None;
        }
        let nodes_tensor = Tensor::cat(nodes_tensors, 0);
        
        let mut edges_tensor = Tensor::zeros([ast.nodes.len(), ast.nodes.len()], device);
        for (src, dst) in ast.edges.iter() {
            edges_tensor = edges_tensor.slice_fill([*src as usize, *dst as usize], 1.0);
        }
        Some(Graph {
            nodes: nodes_tensor.unsqueeze_dim(1),
            edges: edges_tensor,
        })
    }
}

pub fn graph_convolution<B: Backend>(
    graph: Graph<B, Float>,
    weights: Tensor<B, 2>,
    bias: Option<Tensor<B, 1>>,
) -> Graph<B> {
    let normalized_adjacency_matrix = graph.normalized_adjacency_matrix();
    let mut neighbor_features = normalized_adjacency_matrix
        .matmul(graph.nodes)
        .matmul(weights);
    if let Some(bias) = bias {
        neighbor_features = neighbor_features + bias.unsqueeze();
    }
    Graph {
        nodes: neighbor_features,
        edges: graph.edges,
    }
}
