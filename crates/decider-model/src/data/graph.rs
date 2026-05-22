use std::fmt::{Display, Write};

use burn::{
    Tensor,
    prelude::Backend,
    tensor::{BasicOps, Float, Int, TensorKind},
};
use tree_sitter::Tree;

use crate::data::FlattenedAST;

#[derive(Debug, Clone)]
pub struct Graph<B: Backend, D: TensorKind<B> = Float> {
    /// Shape: [num_nodes, num_features]
    pub nodes: Tensor<B, 2, D>,
    /// Shape: [num_nodes, num_nodes]
    /// Adjacency matrix, [target, src]
    pub edges: Tensor<B, 2, Float>,
}

impl<B: Backend, D: TensorKind<B>> Graph<B, D> {
    pub fn new(nodes: Tensor<B, 2, D>, edges: Tensor<B, 2, Float>) -> Self
    where
        D: BasicOps<B>,
    {
        assert_eq!(
            nodes.shape()[0],
            edges.shape()[0],
            "Number of nodes and edges must match"
        );
        assert_eq!(
            edges.shape()[0],
            edges.shape()[1],
            "Adjacency matrix must be square"
        );
        Graph { nodes, edges }
    }

    /// Makes sure that every edge/src->target pair is present at most once.
    pub fn normalized_adjacency_matrix(&self) -> Tensor<B, 2, Float> {
        let degree_matrix = self.edges.clone().sum_dim(1) + 1; // For every node, sum the edges going into it
        let degree_matrix_inv_sqrt = degree_matrix.powf_scalar(-0.5); // Inverse square root of the degree matrix

        self.edges
            .clone() // We accepts some form of amnesia due to no forced self-loops because feature aggregation is still happening and we want to condense features at the end of the day
            .mul(degree_matrix_inv_sqrt.clone()) // Normalize by deg(i) of the target node, https://towardsdatascience.com/graph-convolutional-networks-introduction-to-gnns-24b3f60d6c95/
            .mul(degree_matrix_inv_sqrt.transpose()) // Normalize by deg(j) of all the source nodes j in N_i
    }

    /// Switches the source and target of the edges
    pub fn swap_direction(&mut self) {
        self.edges = self.edges.clone().transpose();
    }

    /// Ensures that for every edge [src, target], there is also an edge [target, src]
    pub fn make_symmetric(&mut self) {
        // Symmetrize the edge matrix: edges = edges + edges^T, but clamp to 1.0 (since it's adjacency)
        let edges_t = self.edges.clone().transpose();
        self.edges = self.edges.clone().max_pair(edges_t);
    }

    /// Removes a node from the graph, removing all edges pointing to and from the node
    pub fn remove_node(&mut self, node_index: usize) -> Result<(), String>
    where
        D: BasicOps<B>,
    {
        if self.nodes.shape()[0] <= 1 {
            return Err(format!(
                "Cannot remove node {node_index} from graph with only one node"
            ));
        }

        self.nodes.inplace(|nodes| {
            let elements = nodes.shape()[0];
            nodes.narrow(0, node_index, elements - 1)
        });
        self.edges.inplace(|edges| {
            let elements = edges.shape()[0];
            edges
                .narrow(0, node_index, elements - 1)
                .narrow(1, node_index, elements - 1)
        });
        Ok(())
    }
}

impl<B: Backend, D: TensorKind<B> + BasicOps<B>> Display for Graph<B, D> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        writeln!(
            f,
            "Graph (Nodes: {}, Edges: {})",
            self.nodes.shape()[0],
            self.edges
                .clone()
                .greater_elem(0.0)
                .int()
                .sum()
                .into_scalar()
        )?;
        Ok(())
    }
}

impl<B: Backend> Graph<B, Int> {
    pub fn from_treesitter_ast(ast: Tree, device: &B::Device) -> Option<Self> {
        Self::from_flattened_ast(&FlattenedAST::from_treesitter_ast(ast), device)
    }

    /// Creates a graph from a [FlattenedAST]
    /// Assumes that the AST edges point from child to parent
    /// Returns a graph with bidirectional edges from child to parent
    pub fn from_flattened_ast(ast: &FlattenedAST, device: &B::Device) -> Option<Self> {
        let nodes_tensors = ast
            .nodes
            .iter()
            .map(|node| Tensor::<_, 1, Int>::from_ints([node.kind_id as i32], device))
            .collect::<Vec<_>>();
        if nodes_tensors.is_empty() {
            return None;
        }
        let nodes_tensor = Tensor::cat(nodes_tensors, 0);

        let mut edges_tensor =
            Tensor::zeros([nodes_tensor.shape()[0], nodes_tensor.shape()[0]], device);
        for (src, dst) in ast.edges.iter() {
            edges_tensor = edges_tensor.slice_fill([(*dst), (*src)], 1.0);
        }
        let mut graph = Graph {
            nodes: nodes_tensor.unsqueeze_dim(1),
            edges: edges_tensor,
        };
        // graph.make_symmetric();  // Remove symmetry if structural information is important
        // Remove root node due increase numerical stability
        graph.remove_node(0).ok()?;
        Some(graph)
    }
}
