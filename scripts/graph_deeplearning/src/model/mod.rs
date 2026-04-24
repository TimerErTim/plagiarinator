use std::{
    fmt::{Display, Write},
    num::NonZeroUsize,
};

use burn::{Tensor, prelude::Backend, tensor::{Int, Slice}};
use fxhash::{FxHashMap, FxHashSet};

/// Undirected Graph Representation
#[derive(Debug, Clone)]
pub struct Graph<B: Backend> {
    /// Shape: [num_nodes, num_features]
    pub nodes: Tensor<B, 2>,
    // Shape: [num_edges, 2]
    // [node_1, node_2]
    pub edges: Tensor<B, 2, Int>,
}

impl<B: Backend> Graph<B> {
    pub fn new(nodes: Tensor<B, 2>, edges: Tensor<B, 2, Int>) -> Self {
        Graph { nodes, edges }
    }

    /// Makes sure that every edge/src->target pair is present at most once.
    pub fn deduplicate_edges(&mut self) where B::IntElem: Into<i64> {
        let mut seen_edges = FxHashSet::<[i64; 2]>::default();
        let mut unique_indices = Vec::with_capacity(self.edges.shape()[0]);
        for (idx, edge) in self.edges.clone().iter_dim(0).enumerate() {
            let edge = edge.squeeze::<1>();
            let n_1 = edge.clone().slice(0..1).into_scalar().into() as i64;
            let n_2 = edge.slice(1..2).into_scalar().into() as i64;
            if !seen_edges.contains(&[n_1, n_2]) && !seen_edges.contains(&[n_2, n_1]) {
                seen_edges.insert([n_1, n_2]);
                unique_indices.push(Slice::index(idx as isize));
            }
        }
        let new_edges = unique_indices.into_iter().map(|idx| self.edges.clone().slice_dim(0, idx));
        self.edges = Tensor::cat(new_edges.collect(), 0);
    }
}

impl<B: Backend> Display for Graph<B> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(
            f,
            "Graph (Nodes: {}, Edges: {})",
            self.nodes.shape()[0],
            self.edges.shape()[0]
        )?;
        writeln!(f, "Nodes: {}", self.nodes)?;
        writeln!(f, "Edges: {}", self.edges)?;
        Ok(())
    }
}

pub fn edge_pseudonodes<B: Backend>(
    device: &B::Device,
    graph: Graph<B>,
    weights: Tensor<B, 2>,
    bias: Tensor<B, 1>,
    neighborhood: NonZeroUsize,
    striding: NonZeroUsize,
) -> Graph<B>
where
    B::IntElem: Into<i64>,
{
    let mut node_edge_index_map = FxHashMap::<usize, FxHashSet<usize>>::default();

    // Ignore neihbourhood and strides for now
    let new_nodes = graph
        .edges
        .clone()
        .iter_dim(0)
        .enumerate()
        .map(|(idx, edge)| {
            let node_indices = edge.squeeze().slice(0..2);
            let n_1 = node_indices.clone().slice(0..1).into_scalar().into() as usize;
            let n_2 = node_indices.clone().slice(1..2).into_scalar().into() as usize;
            node_edge_index_map
                .entry(n_1)
                .or_default()
                .insert(idx);
            node_edge_index_map
                .entry(n_2)
                .or_default()
                .insert(idx);
            let nodes = graph.nodes.clone().select(0, node_indices);
            // src_target shape = [2, node_features]
            // weights shape = [node_features, output_features]
            let node_features = nodes
                .matmul(weights.clone())
                .mean_dim(0)
                .squeeze_dim(0)
                + bias.clone();
            node_features
        });
    let pseudo_nodes = Tensor::stack(new_nodes.collect(), 0);

    let new_edges = graph.edges.iter_dim(0).enumerate().flat_map(|(idx, edge)| {
        let node_indices = edge.squeeze::<1>().slice(0..2);
        let n_1 = node_indices.clone().slice(0..1).into_scalar().into() as usize;
        let n_2 = node_indices.clone().slice(1..2).into_scalar().into() as usize;
        let new_n1  = idx;
        node_edge_index_map.get_mut(&n_1).map(|set| set.remove(&idx));
        node_edge_index_map.get_mut(&n_2).map(|set| set.remove(&idx));
        match (node_edge_index_map.get(&n_1), node_edge_index_map.get(&n_2)) {
            (Some(set_1), Some(set_2)) => {
                set_1.iter().chain(set_2.iter()).map(|&new_n_2| 
                    Tensor::<B, 1, Int>::from_ints([new_n1, new_n_2], device)
                ).collect()
            },
            (Some(set), None) => {
                set.iter().map(|&new_n_2| 
                    Tensor::<B, 1, Int>::from_ints([new_n1, new_n_2], device)
                ).collect()
            },
            (None, Some(set)) => {
                set.iter().map(|&new_n_2| 
                    Tensor::<B, 1, Int>::from_ints([new_n1, new_n_2], device)
                ).collect() 
            }
            (None, None) => {
                Vec::new()
            }
        }
    });
    let pseudo_edges = Tensor::stack(new_edges.collect(), 0);

    return Graph {
        nodes: pseudo_nodes,
        edges: pseudo_edges,
    };
}
