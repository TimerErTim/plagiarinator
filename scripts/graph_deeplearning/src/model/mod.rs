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
    pub fn normalize_edges(&mut self) where B::IntElem: Into<i64> {
        let mut seen_edges = FxHashSet::<[i64; 2]>::default();
        let mut unique_indices = Vec::with_capacity(self.edges.shape()[0]);
        for (idx, edge) in self.edges.clone().iter_dim(0).enumerate() {
            let edge = edge.squeeze::<1>();
            let source = edge.clone().slice(0..1).into_scalar().into() as i64;
            let target = edge.slice(1..2).into_scalar().into() as i64;
            if !seen_edges.contains(&[source, target]) && !seen_edges.contains(&[target, source]) {
                seen_edges.insert([source, target]);
                unique_indices.push(Slice::index(idx as isize));
            }
        }
        let new_edges = unique_indices.into_iter().map(|idx| self.edges.clone().slice_dim(0, idx));
        self.edges = Tensor::cat(new_edges.collect(), 0);
    }

    /// Makes sure that for every x->y edge, there is a y->x edge.
    pub fn make_bidirectional(&mut self) where B::IntElem: Into<i64> {
        let mut edge_states = FxHashMap::<[i64; 2], bool>::default();
        for edge in self.edges.clone().iter_dim(0) {
            let edge = edge.squeeze::<1>();
            let source = edge.clone().slice(0..1).into_scalar().into() as i64;
            let target = edge.clone().slice(1..2).into_scalar().into() as i64;
            // If this edge is in the map, the other direction is already present
            if edge_states.contains_key(&[source, target]) {
                edge_states.entry([source, target]).insert_entry(false);
                continue;
            }
            // Otherwise, set that we need to add the other direction if not already present
            edge_states.entry([target, source]).or_insert(true);
        }
        let device = self.edges.device();
        let new_edges = edge_states.into_iter().filter(|(_, state)| *state).map(|(edge, _)| {
            Tensor::<B, 2, Int>::from_ints([[edge[0], edge[1]]], &device)
        });
        self.edges = Tensor::cat(std::iter::once(self.edges.clone()).chain(new_edges).collect(), 0);
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
    let mut src_index_map = FxHashMap::<usize, Vec<usize>>::default();

    // Ignore neihbourhood and strides for now
    let new_nodes = graph
        .edges
        .clone()
        .iter_dim(0)
        .enumerate()
        .map(|(idx, edge)| {
            let src_target_index = edge.squeeze().slice(0..2);
            src_index_map
                .entry(src_target_index.clone().slice(0..1).into_scalar().into() as usize)
                .or_default()
                .push(idx);
            let src_target = graph.nodes.clone().select(0, src_target_index);
            // src_target shape = [2, node_features]
            // weights shape = [node_features, output_features]
            let node_features = src_target
                .matmul(weights.clone())
                .mean_dim(0)
                .squeeze_dim(0)
                + bias.clone();
            node_features
        });
    let pseudo_nodes = Tensor::stack(new_nodes.collect(), 0);

    let new_edges = graph.edges.iter_dim(0).enumerate().flat_map(|(idx, edge)| {
        let old_target = edge.squeeze::<1>().slice(1..2).into_scalar().into() as usize;
        let new_src = idx;
        let new_targets = src_index_map
            .remove(&old_target)
            .unwrap_or_default()
            .into_iter();
        new_targets
            .map(move |new_target| Tensor::<B, 1, Int>::from_ints([new_src, new_target], device))
    });
    let pseudo_edges = Tensor::stack(new_edges.collect(), 0);

    return Graph {
        nodes: pseudo_nodes,
        edges: pseudo_edges,
    };
}
