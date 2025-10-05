from __future__ import annotations
from dataclasses import dataclass
from tree_sitter import Tree as AST
from parser import load_asts
from pathlib import Path

@dataclass(frozen=True)
class Config:
    pqgram_p: int
    pqgram_q: int
    pqgram_weight: MetricWeight
    ted_weight: MetricWeight


@dataclass(frozen=True)
class MetricWeight:
    x_scale: float
    exponent: float


def similarity_trees(left: AST, right: AST, config: Config) -> float:
    """
    Calculate the similarity between two trees and returns a percentage between 0.0 and 1.0.
    """
    from metrics.common import MetricRunner
    from metrics.pq_gram import pqgram_distance
    from metrics.tree_edit_distance import ast_edit_distance

    ted_metric = MetricRunner("tree_edit_distance", lambda left, right: ast_edit_distance(left, right), config.ted_weight)

    pqgram_metric = MetricRunner("pqgram_distance", lambda left, right: pqgram_distance(left, right, config.pqgram_p, config.pqgram_q), config.pqgram_weight)

    result = MetricRunner.run_all(left, right, pqgram_metric)
    return result.average

def similarity_files(left: Path, right: Path, config: Config, lang_name: str | None = None) -> float:
    left, right = load_asts(left, right, lang_name=lang_name)
    return similarity_trees(left, right, config)
