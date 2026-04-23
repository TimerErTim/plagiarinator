use std::num::{NonZeroU16, NonZeroUsize};

use burn::{
    nn::norm::NormalizationRecordItem,
    prelude::Backend,
    tensor::{ops::IntTensorOps, Float, Int},
    Tensor,
};
use data_loading::dataset_loader::load_dataset;

use crate::model::Graph;

mod layers;
mod model;

pub fn main() {
    type InfBackend = burn::backend::NdArray;
    let device = burn::backend::ndarray::NdArrayDevice::Cpu;
    InfBackend::seed(&device, 32);

    let num_features = 20;
    let num_nodes = 1500;
    let num_edges = 1200;

    let sample_nodes: Tensor<InfBackend, 2, Float> = Tensor::random(
        [num_nodes, num_features],
        burn::tensor::Distribution::Uniform(0.0, 1.0),
        &device,
    );
    let sample_edges: Tensor<InfBackend, 2, Int> = Tensor::random(
        [num_edges, 2],
        burn::tensor::Distribution::Uniform(0.0, num_nodes as f64 - 1.0),
        &device,
    );
    let mut graph = Graph {
        nodes: sample_nodes,
        edges: sample_edges,
    };
    graph.normalize_edges();
    graph.make_bidirectional();

    let mut out_features = 30;
    let mut weights = Tensor::ones([num_features, out_features], &device);
    let mut bias = Tensor::zeros([out_features], &device);

    println!("pre_collapsed graph: {graph}");

    let mut collapsed_graph = model::edge_pseudonodes(
        &device,
        graph,
        weights.clone(),
        bias.clone(),
        NonZeroUsize::new(1).unwrap(),
        NonZeroUsize::new(1).unwrap(),
    );

    for _ in 0..10 {
        let new_out_feat = (out_features as f32 * 1.5) as usize;
        weights = Tensor::ones([out_features, new_out_feat], &device);
        bias = Tensor::zeros([new_out_feat], &device);
        out_features = new_out_feat;
        collapsed_graph = model::edge_pseudonodes(
            &device,
            collapsed_graph,
            weights.clone(),
            bias.clone(),
            NonZeroUsize::new(1).unwrap(),
            NonZeroUsize::new(1).unwrap(),
        );
        collapsed_graph.nodes = burn::tensor::activation::relu(collapsed_graph.nodes);
    }

    println!("collapsed_graph after 10 steps: {collapsed_graph}");

    let dataset = load_dataset("datasets/c_cpp_plagiarism").unwrap();

    println!("Hello, world!");
}
