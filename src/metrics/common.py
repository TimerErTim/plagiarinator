from __future__ import annotations

from tree_sitter import Tree as AST
from dataclasses import dataclass
from typing import Callable
from run import MetricWeight



# Node type normalization: reduce identifiers/literals to canonical tokens
def normalize_node_type(node_type: str) -> str:
    t = node_type.lower()
    if "identifier" in t:
        return "IDENT"
    if "string" in t:
        return "STR"
    if "char" in t and "string" not in t:
        return "CHAR"
    # many grammars use number/float/integer/numeric literal names
    if "number" in t or "float" in t or "int" in t or "decimal" in t or "literal" in t and ("num" in t or "int" in t or "float" in t):
        return "NUM"
    return node_type  # keep original kind for structural signal


class MetricRunner:
    def __init__(
        self, 
        name: str,
        metric_fn: Callable[[AST, AST], float],
        weight: MetricWeight,
    ):
        self.name = name
        self.metric_fn = metric_fn
        self.weight = weight

    def __call__(self, left: AST, right: AST) -> float:
        """
        Applies the metric function and transforms the result so that:
        - input == 0 always returns 1.0 (perfect similarity)
        - As input increases, output drops off to 0, controlled by exponent and x_scale
        - x_scale only affects the "width" of the drop-off, not the output at input=0
        """
        raw = self.metric_fn(left, right)

        # x_scale stretches the x axis: higher x_scale = slower dropoff
        # exponent controls the sharpness of the falloff
        # output = 1.0 if raw == 0, else falls off to 0 as raw increases
        x = raw * self.weight.x_scale if self.weight.x_scale > 0 else raw
        y = 1.0 / ((x + 1) ** self.weight.exponent)
        return max(0.0, min(1.0, y))

    @staticmethod
    def run_all(left: AST, right: AST, *others: MetricRunner) -> CombinedMetricResult:
        """
        Combines multiple MetricRunners' outputs into a single normalized value in [0,1].
        Each runner's output is as per its __call__, and the outputs are combined as a weighted sum,
        with x_scale weights normalized to sum to 1.0.
        """
        outputs = [(runner(left, right), runner.name) for runner in others]
        combined = sum(v for (v, _) in outputs)
        # Clamp to [0.0, 1.0]
        average = max(0.0, min(1.0, combined / len(outputs)))
        return CombinedMetricResult(results=[MetricResult(name=name, value=value) for (value, name) in outputs], average=average)

@dataclass(frozen=True)
class MetricResult:
    name: str
    value: float

@dataclass(frozen=True)
class CombinedMetricResult:
    results: list[MetricResult]
    average: float
