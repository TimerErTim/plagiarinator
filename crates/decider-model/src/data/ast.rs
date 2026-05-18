use serde::{Deserialize, Serialize};

#[derive(Debug, Clone)]
pub struct FlattenedAST {
    pub nodes: Vec<ASTNode>,
    /// Edges between nodes, [child, parent]
    pub edges: Vec<(usize, usize)>,
}

#[derive(Debug, Clone)]
pub struct ASTNode {
    pub kind_id: u16,
    pub kind_name: String,
    pub is_named: bool,
    pub range: FileRange,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct FileRange {
    pub start_line: usize,
    pub start_column: usize,
    pub end_line: usize,
    pub end_column: usize,
    pub start_byte: usize,
    pub end_byte: usize,
}

impl FileRange {
    pub fn from_treesitter_range(range: tree_sitter::Range) -> Self {
        Self { start_line: range.start_point.row, start_column: range.start_point.column, end_line: range.end_point.row, end_column: range.end_point.column, start_byte: range.start_byte, end_byte: range.end_byte }
    }
}

impl ASTNode {
    pub fn from_treesitter_node(node: tree_sitter::Node) -> Self {
        Self {
            kind_id: node.kind_id(),
            kind_name: node.kind().to_string(),
            is_named: node.is_named(),
            range: FileRange::from_treesitter_range(node.range()),
        }
    }
}

impl FlattenedAST {
    // Assumes that the AST edges point from tree child to tree parent
    pub fn from_treesitter_ast(ast: tree_sitter::Tree) -> Self {
        let mut nodes = Vec::with_capacity(ast.root_node().descendant_count());
        let mut edges = Vec::with_capacity(ast.root_node().descendant_count());
        let mut cursor = ast.walk();
        let mut walking_stack = vec![(0, ast.root_node())];

        // Add the root node
        nodes.push(ASTNode::from_treesitter_node(cursor.node()));

        while cursor.goto_next_sibling() {
            let node = cursor.node();
            nodes.push(ASTNode::from_treesitter_node(node));
            walking_stack.push((nodes.len() - 1, node));
        }

        while let Some((parent_idx, node)) = walking_stack.pop() {
            cursor.reset(node);
            if !cursor.goto_first_child() {
                continue;
            }
            loop {
                let child_node = cursor.node();
                nodes.push(ASTNode::from_treesitter_node(child_node));
                edges.push((nodes.len() - 1, parent_idx));
                walking_stack.push((nodes.len() - 1, child_node));
                if !cursor.goto_next_sibling() {
                    break;
                }
            }
        }

        FlattenedAST { nodes, edges }
    }
}
