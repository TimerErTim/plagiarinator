use std::fs::File;

use data_loading::dataset_loader::load_dataset;
use fxhash::FxHashMap;
use graph_deeplearning::parsing::parse_cpp_to_tree;

fn main() {
    let dataset_path = "datasets/c_cpp_plagiarism";
    let dataset = load_dataset(dataset_path).unwrap();
    let mut node_kind_distr = FxHashMap::<u16, (String, usize)>::default();
    for pairs in dataset.cpp_dataset.authentic_pairs.iter().chain(dataset.cpp_dataset.plagiarized_pairs.iter()) {
        for file in [&pairs.left_path, &pairs.right_path] {
            let tree = parse_cpp_to_tree(File::open(file).expect(format!("Failed to open file: {}", file.display()).as_str()));
            let mut cursor = tree.walk();
            let mut stack = vec![cursor.node()];
            while cursor.goto_next_sibling() {
                stack.push(cursor.node());
            }
            while let Some(node) = stack.pop() {
                let node_kind_id = node.kind_id();
                cursor.reset(node);
                if node.is_named() {
                    node_kind_distr.entry(node_kind_id).or_insert((node.kind().to_string(), 0)).1 += 1;
                }
                if !cursor.goto_first_child() {
                    continue;
                }
                stack.push(cursor.node());
                while cursor.goto_next_sibling() {
                    stack.push(cursor.node());
                }
            }
        }
    }
    let highest_node_kind_id = node_kind_distr.iter().max_by_key(|(id, (_, count))| *id).unwrap().0;
    println!("Highest node kind ID: {}", highest_node_kind_id);
    let mut node_kind_distr = node_kind_distr.iter().collect::<Vec<_>>();
    node_kind_distr.sort_by_key(|(id, (_, count))| *id);
    for (node_kind_id, (node_kind, count)) in node_kind_distr {
        println!("Node kind ID: {}, Node kind: {}, Count: {}", node_kind_id, node_kind, count);
    }
}