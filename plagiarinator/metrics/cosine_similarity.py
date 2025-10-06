from collections import Counter
from typing import Tuple
from metrics.common import normalize_node_type
import numpy as np
from tree_sitter import Node, Tree as AST

def node_cosine_similarity(left: AST, right: AST) -> float:
    l_node_counts, _ = walk_ast_counts(left.root_node)
    r_node_counts, _ = walk_ast_counts(right.root_node)
    return 1.0 - _cosine_similarity(l_node_counts, r_node_counts)

def bigram_cosine_similarity(left: AST, right: AST) -> float:
    _, l_bigram_counts = walk_ast_counts(left.root_node)
    _, r_bigram_counts = walk_ast_counts(right.root_node)
    return 1.0 - _cosine_similarity(l_bigram_counts, r_bigram_counts)

def walk_ast_counts(root: Node) -> Tuple[Counter, Counter]:
    """
    Traverse AST and build:
    - node_counts: Counter[node.type]
    - bigram_counts: Counter[(parent.type, child.type)]
    """
    node_counts = Counter()
    bigram_counts = Counter()

    def dfs(node: Node, parent_type: str | None = None):
        nt = normalize_node_type(node.type)
        node_counts[nt] += 1
        if parent_type is not None:
            bigram_counts[(parent_type, nt)] += 1
        for ch in node.children:
            dfs(ch, nt)

    dfs(root, None)
    return node_counts, bigram_counts

def _cosine_similarity(counter_a: Counter, counter_b: Counter) -> float:
    if not counter_a and not counter_b:
        return 1.0
    # unified vocabulary
    keys = list(set(counter_a.keys()) | set(counter_b.keys()))
    va = np.array([counter_a.get(k, 0.0) for k in keys], dtype=float)
    vb = np.array([counter_b.get(k, 0.0) for k in keys], dtype=float)
    denom = (np.linalg.norm(va) * np.linalg.norm(vb))
    if denom == 0:
        return 0.0
    return float(np.dot(va, vb) / denom)