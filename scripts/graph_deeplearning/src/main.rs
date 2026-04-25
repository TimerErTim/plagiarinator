use std::{fs::File, io::BufReader, num::{NonZeroU16, NonZeroUsize}};

use burn::{
    nn::norm::NormalizationRecordItem,
    prelude::Backend,
    tensor::{ops::IntTensorOps, Float, Int},
    Tensor,
};
use data_loading::dataset_loader::load_dataset;

use graph_deeplearning::{layers::{GraphCompressionLayer, GraphCompressionLayerConfig}, model::Graph, parsing::parse_cpp_to_tree};

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

    let mut out_features = 30;
    let mut layer = GraphCompressionLayerConfig::new(num_features, out_features).init(&device);

    println!("pre_collapsed graph: {graph}");

    let mut collapsed_graph = layer.forward(graph);

    for _ in 0..10 {
        let new_out_feat = (out_features as f32 * 1.5) as usize;
        layer = GraphCompressionLayerConfig::new(out_features, new_out_feat).init(&device);
        out_features = new_out_feat;
        collapsed_graph = layer.forward(collapsed_graph);
        collapsed_graph.nodes = burn::tensor::activation::relu(collapsed_graph.nodes);
    }

    println!("collapsed_graph after 10 steps: {collapsed_graph}");

    let dataset = load_dataset("datasets/c_cpp_plagiarism").unwrap();


    for pairs in dataset.cpp_dataset.plagiarized_pairs.iter() {
        let ast = parse_cpp_to_tree(BufReader::new(File::open(pairs.left_path.clone()).unwrap()));
        println!("loading ast left_path: {}", pairs.left_path.display());
        ast.print_dot_graph(&File::create(pairs.left_path.clone().with_extension("dot")).unwrap());
        let left_graph = Graph::<InfBackend>::from_treesitter_ast(ast, device);
        println!("left_graph: {left_graph}");
    }
    println!("Hello, world!");
}
