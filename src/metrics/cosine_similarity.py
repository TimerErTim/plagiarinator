from collections import Counter


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