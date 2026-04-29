#[derive(Debug, Clone)]
pub struct FlattenedAST {
    pub nodes: Vec<ASTNode>,
    /// Edges between nodes, [src, target]
    pub edges: Vec<(usize, usize)>,
}

#[derive(Debug, Clone)]
pub struct ASTNode {
    pub kind_id: u16,
    pub kind_name: String,
    pub is_named: bool,
    pub range: tree_sitter::Range
}

impl ASTNode {
    pub fn from_treesitter_node(node: tree_sitter::Node) -> Self {
        Self {
            kind_id: node.kind_id(),
            kind_name: node.kind().to_string(),
            is_named: node.is_named(),
            range: node.range(),
        }
    }
}

impl FlattenedAST {
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

        FlattenedAST { nodes, edges: edges }
    }
}
