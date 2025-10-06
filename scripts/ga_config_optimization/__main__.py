from typing import Collection, Iterator, List
import numpy as np
import radiate as rd

from plagiarinator.run import Config, similarity_trees, MetricWeight
from .dataset_loader import load_balanced_pairs

rd.random.seed(42)

from pathlib import Path


def batched_shuffled[T](items: Collection[T], batch_size=250, seed=42) -> Iterator[List[T]]:
    """
    Wraps an iterator, shuffles all items, and yields batches of size batch_size.
    Repeats infinitely, reshuffling each epoch.
    """
    rng = np.random.default_rng(seed)
    while True:
        rng.shuffle(items)
        for i in range(0, len(items), batch_size):
            yield items[i:i+batch_size]

file_path = Path(__file__).resolve()
dataset_root = file_path.parent.parent.parent / "datasets" / "c_cpp_plagiarism"
print(f"Dataset root: {dataset_root}")
dataset = load_balanced_pairs(dataset_root)
batched_dataset = batched_shuffled(dataset)

def evaluate_config(config: Config) -> float:
    batch = next(batched_dataset)
    total_error = 0.0
    for (left, right), target_plagiarism, (left_path, right_path) in batch:
        value = similarity_trees(left, right, config)
        error = abs(value - target_plagiarism)
        total_error += error
    print(f"Total error: {total_error}")
    return total_error / len(batch)


metric_count = 5
codec = rd.FloatCodec.from_chromosomes([
    rd.chromosome.float(length=3, init_range=(1.0, 4.0), bounds=(1.0, 8.0)),
    rd.chromosome.float(length=metric_count, init_range=(0.0, 1.0), bounds=(0.0, 1.0)),
    rd.chromosome.float(length=metric_count, init_range=(0.1, 5.0), bounds=(0.1, 10.0)),
    rd.chromosome.float(length=metric_count, init_range=(0.1, 5.0), bounds=(0.1, 7.5)),
])

def parse_config_from_genotype(genotype: list[list[float]]) -> Config:
    """
    Decodes a Genotype into its representation.
    :param genotype: A Genotype instance to decode.
    :return: The decoded representation of the Genotype.
    """
    metric_weights = []
    for (w, x_scale, exponent) in zip(genotype[1], genotype[2], genotype[3]):
        metric_weights.append(MetricWeight(w, x_scale, exponent))
    pqgram_p = int(genotype[0][0])
    pqgram_q = int(genotype[0][1])
    wl_subtree_jaccard_rounds = int(genotype[0][2])
    return Config(
        pqgram_p=pqgram_p,
        pqgram_q=pqgram_q,
        wl_subtree_jaccard_rounds=wl_subtree_jaccard_rounds,
        pqgram_weight=metric_weights[0],
        ted_weight=metric_weights[1],
        node_cosine_weight=metric_weights[2],
        bigram_cosine_weight=metric_weights[3],
        wl_subtree_jaccard_weight=metric_weights[4],
    )

engine = rd.GeneticEngine(
    codec=codec,
    fitness_func=lambda x: evaluate_config(parse_config_from_genotype(x)),
    objectives="min",
    executor=rd.WorkerPool(),
)

print(f"Running engine...")
result = engine.run(rd.ScoreLimit(0.01), log=True)

best_config = parse_config_from_genotype(result.value())    
print(f"\nBest solution found: {best_config}")
print(f"Fitness: {result.score()}")
print(f"Generations completed: {result.index()}")
print(f"Duration: {result.duration()}")

print(result.metrics())