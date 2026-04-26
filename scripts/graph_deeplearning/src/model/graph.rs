use std::{
    fmt::{Display, Write},
    num::NonZeroUsize,
};

use burn::{
    Tensor, prelude::Backend, tensor::{Float, Int, Slice, TensorKind}
};
use fxhash::{FxHashMap, FxHashSet};
use tree_sitter::Tree;

#[derive(Debug, Clone)]
pub struct Graph<B: Backend, D: TensorKind<B> = Float> {
    /// Shape: [num_nodes, num_features]
    pub nodes: Tensor<B, 2, D>,
    // Shape: [num_edges, 2]
    // [source_node, target_node]
    pub edges: Option<Tensor<B, 2, Int>>,
}

impl<B: Backend, D: TensorKind<B>> Graph<B, D> {
    pub fn new(nodes: Tensor<B, 2, D>, edges: Tensor<B, 2, Int>) -> Self {
        Graph { nodes, edges: Some(edges) }
    }

    pub fn unconnected(nodes: Tensor<B, 2, D>) -> Self {
        Graph { nodes, edges: None }
    }

    /// Makes sure that every edge/src->target pair is present at most once.
    pub fn normalize_edges(&mut self)
    where
        B::IntElem: Into<i64>,
    {
        let Some(edges) = self.edges.as_ref() else {
            return;
        };

        let mut seen_edges = FxHashSet::<[i64; 2]>::default();
        let mut unique_indices = Vec::with_capacity(edges.shape()[0]);
        for (idx, edge) in edges.clone().iter_dim(0).enumerate() {
            let edge = edge.squeeze::<1>();
            let source = edge.clone().slice(0..1).into_scalar().into() as i64;
            let target = edge.slice(1..2).into_scalar().into() as i64;
            if !seen_edges.contains(&[source, target]) {
                seen_edges.insert([source, target]);
                unique_indices.push(Slice::index(idx as isize));
            }
        }
        let new_edges = unique_indices
            .into_iter()
            .map(|idx| edges.clone().slice_dim(0, idx));
        self.edges = Some(Tensor::cat(new_edges.collect(), 0));
    }

    /// Makes sure that for every x->y edge, there is a y->x edge.
    pub fn make_bidirectional(&mut self)
    where
        B::IntElem: Into<i64>,
    {
        let Some(edges) = self.edges.as_ref() else {
            return;
        };
        let mut edge_states = FxHashMap::<[i64; 2], bool>::default();
        for edge in edges.clone().iter_dim(0) {
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
        let device = edges.device();
        let new_edges = edge_states
            .into_iter()
            .filter(|(_, state)| *state)
            .map(|(edge, _)| Tensor::<B, 2, Int>::from_ints([[edge[0], edge[1]]], &device));
        self.edges = Some(Tensor::cat(
            std::iter::once(edges.clone())
                .chain(new_edges)
                .collect::<Vec<_>>(),
            0,
        ));
    }
}

impl<B: Backend> Display for Graph<B> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(
            f,
            "Graph (Nodes: {}, Edges: {})",
            self.nodes.shape()[0],
            self.edges.as_ref().map(|edges| edges.shape()[0]).unwrap_or(0)
        )?;
        writeln!(f, "Nodes: {}", self.nodes)?;
        if let Some(edges) = self.edges.as_ref() {
            writeln!(f, "Edges: {}", edges)?;
        }
        Ok(())
    }
}

impl<B: Backend> Graph<B, Int> {
    pub fn from_treesitter_ast(ast: Tree, device: &B::Device) -> Option<Self> {
        let mut nodes = Vec::with_capacity(ast.root_node().descendant_count());
        let mut edges = Vec::with_capacity(ast.root_node().descendant_count());
        let mut cursor = ast.walk();
        let mut walking_stack = vec![(0, cursor.node())];
        while cursor.goto_next_sibling() {
            let node = cursor.node();
            let node_kind = node.kind_id();
            nodes.push(node_kind);
            walking_stack.push((nodes.len() - 1, node));
        }
        while let Some((nodes_idx, node)) = walking_stack.pop() {
            cursor.reset(node);
            if !cursor.goto_first_child() {
                continue;
            }
            loop {
                let child_node = cursor.node();
                let child_node_kind = child_node.kind_id();
                nodes.push(child_node_kind);
                edges.push([(nodes.len() - 1) as i32, nodes_idx as i32]);
                walking_stack.push((nodes.len() - 1, child_node));
                if !cursor.goto_next_sibling() {
                    break;
                }
            }
        }
        let nodes_tensors = nodes.iter().map(|node| Tensor::<_, 1, Int>::from_ints([*node as i32 - 1], device)).collect::<Vec<_>>();
        if nodes_tensors.is_empty() {
            return None;
        }
        let nodes_tensor = Tensor::cat(nodes_tensors, 0);
        // Somehow first edge is always [0, 0] so we skip it
        let edges_tensor = Tensor::stack::<2>(edges.iter().skip(1).map(|edge| Tensor::<_, 1, Int>::from_ints(edge.as_slice(), device)).collect(), 0);
        Some(Graph {
            nodes: nodes_tensor.unsqueeze(),
            edges: Some(edges_tensor),
        })
    }
}

pub fn edge_pseudonodes<B: Backend>(
    graph: Graph<B, Float>,
    weights: Tensor<B, 2>,
    bias: Option<Tensor<B, 1>>,
    orphan_weights: Tensor<B, 2>,
    orphan_bias: Option<Tensor<B, 1>>,
) -> Graph<B>
where
    B::IntElem: Into<i64>,
{
    let mut src_index_map = FxHashMap::<usize, Vec<usize>>::default();
    let mut orphan_nodes = FxHashSet::<usize>::with_capacity_and_hasher(
        graph.nodes.shape()[0] as usize,
        Default::default(),
    );
    orphan_nodes.extend(0..graph.nodes.shape()[0]);
    let Some(edges) = graph.edges else {
        // The graph has no edges, no new collapsed pseudonodes possible, so we simply map distribute orphan nodes to pseudo nodes
        let mut pseudo_nodes = graph.nodes.matmul(orphan_weights.clone());
        if let Some(bias) = orphan_bias.as_ref() {
            pseudo_nodes = pseudo_nodes + bias.clone().unsqueeze();
        }
        return Graph {
            nodes: pseudo_nodes,
            edges: None,
        };
    };
    let edges_device = edges.device();
    let nodes_device = graph.nodes.device();

    // Ignore neihbourhood and strides for now
    let new_nodes = edges
        .clone()
        .iter_dim(0)
        .enumerate()
        .map(|(idx, edge)| {
            let src_target_index = edge.squeeze().slice(0..2);
            src_index_map
                .entry(src_target_index.clone().slice(0..1).into_scalar().into() as usize)
                .or_default()
                .push(idx);
            let src_index = src_target_index.clone().slice(0..1).into_scalar().into() as usize;
            orphan_nodes.remove(&src_index);
            let target_index = src_target_index.clone().slice(1..2).into_scalar().into() as usize;
            orphan_nodes.remove(&target_index);
            let src_target = graph.nodes.clone().select(0, src_target_index);
            // src_target shape = [2, node_features]
            // weights shape = [node_features, output_features]
            let mut node_features = src_target
                .matmul(weights.clone())
                .mean_dim(0)
                .squeeze_dim(0);
            if let Some(bias) = bias.as_ref() {
                node_features = node_features + bias.clone();
            }
            node_features
        });
    let new_nodes = new_nodes.collect::<Vec<_>>();
    let mut pseudo_nodes = Tensor::stack(new_nodes, 0);
    // Distribute orphan nodes to pseudo nodes
    if !orphan_nodes.is_empty() {
        let orphan_indices: Tensor<B, 1, Int> = Tensor::from_ints(
            orphan_nodes
                .into_iter()
                .map(|idx| idx as i32)
                .collect::<Vec<_>>()
                .as_slice(),
            &nodes_device,
        );
        let orphan_nodes = graph.nodes.clone().select(0, orphan_indices);
        let mut orphan_distr_features = orphan_nodes.matmul(orphan_weights.clone());
        if let Some(bias) = orphan_bias.as_ref() {
            orphan_distr_features = orphan_distr_features + bias.clone().unsqueeze();
        }
        pseudo_nodes = pseudo_nodes.clone()
            + orphan_distr_features.sum_dim(0) / pseudo_nodes.shape()[0] as f64;
    }

    let new_edges = edges.clone().iter_dim(0).enumerate().flat_map(|(idx, edge)| {
        let old_target = edge.squeeze::<1>().slice(1..2).into_scalar().into() as usize;
        let new_src = idx;
        let new_targets = src_index_map
            .remove(&old_target)
            .unwrap_or_default()
            .into_iter();
        let edges_device = edges_device.clone();
        new_targets.map(move |new_target| {
            Tensor::<B, 1, Int>::from_ints([new_src, new_target], &edges_device)
        })
    });
    let new_edges = new_edges.collect::<Vec<_>>();
    let pseudo_edges = if new_edges.is_empty() {
        None
    } else {
        Some(Tensor::stack(new_edges, 0))
    };

    return Graph {
        nodes: pseudo_nodes,
        edges: pseudo_edges,
    };
}
