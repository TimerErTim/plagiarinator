from metrics.common import normalize_node_type
from pqgrams import PQGram
from tree_sitter import Tree as AST
from pqgrams.tree import Node as PQGramNode

def _build_pqgram_tree(ast: AST) -> PQGramNode:
    """
    Converts a tree_sitter AST node into an PQGramNode using a stack (non-recursive),
    using normalized node types (node.type).
    """

    if ast is None:
        return None

    root_node = PQGramNode(normalize_node_type(ast.root_node.type))
    stack = [(root_node, ast.root_node)]

    while stack:
        (target_node, source_ast_node) = stack.pop()
        for child in source_ast_node.children:
            child_node = PQGramNode(normalize_node_type(child.type))
            target_node.addkid(child_node)
            stack.append((child_node, child))

    return root_node

def pqgram_distance(left: AST, right: AST, p: int, q: int) -> float:
    left_pqgram_tree, right_pqgram_tree = (_build_pqgram_tree(left), _build_pqgram_tree(right))
    left_pqgram, right_pqgram = (PQGram.Profile(left_pqgram_tree, p, q), PQGram.Profile(right_pqgram_tree, p, q))
    return left_pqgram.edit_distance(right_pqgram)