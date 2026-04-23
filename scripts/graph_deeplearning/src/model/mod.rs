use std::{
    fmt::{Display, Write},
    num::NonZeroUsize,
};

use burn::{prelude::Backend, tensor::Int, Tensor};
use fxhash::FxHashMap;

#[derive(Debug, Clone)]
pub struct Graph<B: Backend> {
    /// Shape: [num_nodes, num_features]
    pub nodes: Tensor<B, 2>,
    // Shape: [num_edges, 2]
    // [source_node, target_node]
    pub edges: Tensor<B, 2, Int>,
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
