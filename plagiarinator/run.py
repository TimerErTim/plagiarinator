from __future__ import annotations
from dataclasses import dataclass
from tree_sitter import Tree as AST
from parser import load_asts
from pathlib import Path
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from metrics.common import CombinedMetricResult


@dataclass(frozen=True)
class Config:
    pqgram_p: int
    pqgram_q: int
    wl_subtree_jaccard_rounds: int
    pqgram_weight: MetricWeight
    ted_weight: MetricWeight
    node_cosine_weight: MetricWeight
    bigram_cosine_weight: MetricWeight
    wl_subtree_jaccard_weight: MetricWeight


@dataclass(frozen=True)
class MetricWeight:
    x_scale: float
    exponent: float
    weight: float


def similarity_trees(left: AST, right: AST, config: Config) -> CombinedMetricResult:
    """
    Calculate the similarity between two trees and returns a percentage between 0.0 and 1.0.
    """
    from metrics.common import MetricRunner
    from metrics.pq_gram import pqgram_distance
    from metrics.tree_edit_distance import ast_edit_distance
    from metrics.cosine_similarity import bigram_cosine_similarity, node_cosine_similarity
    from metrics.jaccard_multiset import wl_subtree_jaccard
    from functools import partial

    ted_metric = MetricRunner("tree_edit_distance",
                              ast_edit_distance, config.ted_weight)

    pqgram_metric = MetricRunner(
        "pqgram_distance",
        partial(pqgram_distance, p=config.pqgram_p, q=config.pqgram_q),
        config.pqgram_weight
    )

    node_cosine_metric = MetricRunner(
        "node_cosine_similarity", node_cosine_similarity, config.node_cosine_weight)
    bigram_cosine_metric = MetricRunner(
        "bigram_cosine_similarity", bigram_cosine_similarity, config.bigram_cosine_weight)

    wl_subtree_jaccard_metric = MetricRunner("wl_subtree_jaccard",
        partial(wl_subtree_jaccard, rounds=config.wl_subtree_jaccard_rounds),
        config.wl_subtree_jaccard_weight
    )

    result = MetricRunner.run_all(
        left, right, pqgram_metric, node_cosine_metric, bigram_cosine_metric, wl_subtree_jaccard_metric
    )
    return result


def similarity_files(left: Path, right: Path, config: Config, lang_name: str | None = None) -> CombinedMetricResult:
    left, right = load_asts(left, right, lang_name=lang_name)
    return similarity_trees(left, right, config)
