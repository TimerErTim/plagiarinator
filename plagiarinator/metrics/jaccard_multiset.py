from collections import Counter
from tree_sitter import Node
from metrics.common import normalize_node_type
import hashlib

def wl_subtree_jaccard(left: Node, right: Node, rounds: int = 1) -> float:
    l_counts = _wl_subtree_hashes(left.root_node, rounds)
    r_counts = _wl_subtree_hashes(right.root_node, rounds)
    return 1.0 - _jaccard_multiset(l_counts, r_counts)

# WL-style subtree hashing
def _wl_subtree_hashes(root: Node, rounds: int = 1) -> Counter:
    """
    Compute multiset of subtree hashes using a simple WL procedure.
    rounds=0 gives purely (type)-based; rounds>=1 incorporates neighbors.
    Returns Counter of hex digests (as strings).
    """
    # First, gather nodes in post-order so children processed before parents.
    nodes = []

    def post(node: Node):
        for ch in node.children:
            post(ch)
        nodes.append(node)

    post(root)

    # initial labels: normalized node types
    labels: dict[int, str] = {}
    for n in nodes:
        labels[n.id] = normalize_node_type(n.type)

    # WL refinement
    for _ in range(rounds):
        new_labels: dict[int, str] = {}
        for n in nodes:
            child_labels = [labels[ch.id] for ch in n.children]
            sig = labels[n.id] + "|" + ",".join(sorted(child_labels))
            # hash the signature for stability
            h = hashlib.blake2b(sig.encode("utf-8"), digest_size=16).hexdigest()
            new_labels[n.id] = h
        labels = new_labels

    # Collect subtree hash multiset (the final labels are the subtree IDs)
    counts = Counter(labels.values())
    return counts

def _jaccard_multiset(counter_a: Counter, counter_b: Counter) -> float:
    if not counter_a and not counter_b:
        return 1.0
    # multiset Jaccard = sum(min)/sum(max)
    all_keys = set(counter_a.keys()) | set(counter_b.keys())
    inter = sum(min(counter_a.get(k, 0), counter_b.get(k, 0)) for k in all_keys)
    union = sum(max(counter_a.get(k, 0), counter_b.get(k, 0)) for k in all_keys)
    if union == 0:
        return 0.0
    return inter / union