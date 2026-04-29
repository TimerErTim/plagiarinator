use std::fs::File;

use data_loading::dataset_loader::load_dataset;
use graph_deeplearning::{loading::parse_cpp_to_tree, model::Graph};

pub fn main() {
    type Backend = burn::backend::ndarray::NdArray;
    let dataset = load_dataset("datasets/c_cpp_plagiarism").unwrap();
    let device = burn::backend::ndarray::NdArrayDevice::default();
    let ast = parse_cpp_to_tree(File::open(dataset.cpp_dataset.plagiarized_pairs[0].left_path.clone()).unwrap());
    let graph = Graph::<Backend, _>::from_treesitter_ast(ast, &device).unwrap();
    println!("{:?}", graph);
}