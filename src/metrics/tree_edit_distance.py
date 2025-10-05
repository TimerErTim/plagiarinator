from tree_sitter import Tree as AST
from apted.helpers import Tree as AptedTree
import apted
from metrics.common import normalize_node_type

def ast_edit_distance(left: AST, right: AST) -> float:
    left_apted, right_apted = (_build_apted_tree(left), _build_apted_tree(right))
    apted_instance = apted.APTED(left_apted, right_apted)

    ted = apted_instance.compute_edit_distance()
    max_distance = count_nodes(left_apted) + count_nodes(right_apted)
    return ted / max_distance


def count_nodes(tree: AptedTree) -> int:
    stack = [tree]
    count = 1
    while stack:
        node = stack.pop()
        for child in node.children:
            stack.append(child)
            count += 1
    return count

def _build_apted_tree(ast: AST) -> AptedTree:
    """
    Converts a tree_sitter AST node into an AptedTree using a stack (non-recursive),
    using normalized node types (node.type).
    """

    if ast is None:
        return None

    root_node = AptedTree(normalize_node_type(ast.root_node.type))
    stack = [(root_node, ast.root_node)]

    while stack:
        (target_node, source_ast_node) = stack.pop()
        for child in source_ast_node.children:
            child_node = AptedTree(normalize_node_type(child.type))
            target_node.children.append(child_node)
            stack.append((child_node, child))

    return root_node
