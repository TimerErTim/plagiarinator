use std::io::{BufReader, Read};

use crate::model::Graph;

pub fn parse_cpp_to_tree(reader: impl Read) -> tree_sitter::Tree {
    let mut parser = tree_sitter::Parser::new();
    parser.set_language(&tree_sitter_cpp::LANGUAGE.into()).expect("Failed to set language to C++");
    let tree = parser.parse(reader.bytes().map(|b| b.expect("Failed to read bytes")).collect::<Vec<u8>>().as_slice(), None).expect("Failed to parse C++ code");
    tree
}


