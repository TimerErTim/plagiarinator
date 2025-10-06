from __future__ import annotations
from pathlib import Path
from typing import Iterable, Iterator, Optional, Tuple, List, Dict, Set
from itertools import combinations
import random

# We import parser helpers to build ASTs consistently with the rest of the project
from tree_sitter import Tree as AST
try:
    # When executed from project root where src is on path
    from parser import parse_code, infer_language  # type: ignore
except Exception:
    # Fallback: add project src to sys.path when running from scripts/
    import sys
    project_root = Path(__file__).resolve().parents[2]
    src_dir = project_root / "src"
    if str(src_dir) not in sys.path:
        sys.path.insert(0, str(src_dir))
    from parser import parse_code, infer_language  # type: ignore


def _read_ground_truth_static(gt_path: Path) -> Dict[str, List[List[str]]]:
    """
    Parse ground-truth-static-anon.txt into a mapping:
      assignment_id -> list of plagiarized groups (each group is a list of student ids)

    File format (per datasets/c_cpp_plagiarism/readme.txt):
      - <assignment-id>
      student123,student456
      student789,student012,student345
      - <next-assignment-id>
      ...
    """
    if not gt_path.exists():
        raise FileNotFoundError(f"Ground truth file not found: {gt_path}")

    assignment_to_groups: Dict[str, List[List[str]]] = {}
    current_assignment: Optional[str] = None

    for raw_line in gt_path.read_text(encoding="utf-8").splitlines():
        line = raw_line.strip()
        if not line:
            continue
        if line.startswith("-"):
            # New assignment header line: "- A2016/Z1/Z1"
            # Keep the part after the dash and space
            header = line[1:].strip()
            current_assignment = header
            assignment_to_groups.setdefault(current_assignment, [])
            continue
        if current_assignment is None:
            # Malformed file (group line without prior assignment header)
            continue
        # Comma-separated list of students; keep only non-empty tokens
        students = [s.strip() for s in line.split(",") if s.strip()]
        if len(students) >= 2:
            assignment_to_groups[current_assignment].append(students)

    return assignment_to_groups


def _assignment_dir(dataset_root: Path, assignment_id: str) -> Path:
    """
    Convert an assignment id like "A2016/Z1/Z1" to a path under datasets src tree.
    Expected layout: <dataset_root>/src/A2016/Z1/Z1
    """
    return dataset_root.joinpath("src").joinpath(*assignment_id.split("/"))


def _find_student_files(assignment_path: Path) -> Dict[str, Path]:
    """
    Find student source files in the given assignment directory, mapped by student id.
    Looks for files named like "studentXXXX.*".
    """
    if not assignment_path.exists():
        return {}
    id_to_path: Dict[str, Path] = {}
    for child in assignment_path.iterdir():
        if not child.is_file():
            continue
        name = child.name
        if not name.startswith("student"):
            continue
        # Strip extension, keep the leading token for student id
        student_id = name.split(".")[0]
        id_to_path[student_id] = child
    return id_to_path


def _all_pairs(items: Iterable[str]) -> Iterator[Tuple[str, str]]:
    for a, b in combinations(sorted(set(items)), 2):
        yield a, b


def _positive_pairs_for_assignment(groups: List[List[str]]) -> Set[Tuple[str, str]]:
    """
    For each group, generate all unique unordered pairs inside the group.
    """
    pos: Set[Tuple[str, str]] = set()
    for group in groups:
        for a, b in _all_pairs(group):
            pos.add((a, b))
    return pos


def _candidate_negative_pairs(all_students: Iterable[str], positive_pairs: Set[Tuple[str, str]]) -> List[Tuple[str, str]]:
    """
    All unique unordered pairs among students, excluding those marked positive.
    """
    negatives: List[Tuple[str, str]] = []
    for a, b in _all_pairs(all_students):
        if (a, b) not in positive_pairs:
            negatives.append((a, b))
    return negatives


def _parse_file_to_ast(path: Path) -> AST:
    lang = infer_language(path)
    if lang is None:
        raise ValueError(f"Cannot infer language for file: {path}")
    code = path.read_bytes()
    return parse_code(code, lang)


def load_balanced_pairs(
    dataset_root: Path | str,
    ground_truth_path: Optional[Path | str] = None
) -> List[Tuple[Tuple[AST, AST], float, Tuple[Path, Path]]]:
    """
    Build a globally 50/50 balanced list of pairs across assignments.

    Returns a list of entries, each as:
      ((left_ast, right_ast), y_label, (left_path, right_path))

    Where y_label is 1.0 for plagiarized pairs (from ground truth) and 0.0 for normal pairs.
    """
    dataset_root = Path(dataset_root)
    gt_path = Path(ground_truth_path) if ground_truth_path is not None else dataset_root / "ground-truth-static-anon.txt"
    rng = random.Random(42)

    assignment_groups = _read_ground_truth_static(gt_path)

    # Collect positive pairs per assignment and map of available student files
    assignment_to_id_and_path: Dict[str, Dict[str, Path]] = {}
    assignment_to_positives: Dict[str, Set[Tuple[str, str]]] = {}

    for assignment_id, groups in assignment_groups.items():
        assignment_path = _assignment_dir(dataset_root, assignment_id)
        id_to_path = _find_student_files(assignment_path)
        if not id_to_path:
            continue
        assignment_to_id_and_path[assignment_id] = id_to_path

        positives = _positive_pairs_for_assignment(groups)
        # Keep only pairs where both students are present in filesystem
        positives = {
            pair
            for pair in positives
            if pair[0] in id_to_path and pair[1] in id_to_path
        }
        assignment_to_positives[assignment_id] = positives

    # Flatten positives and negatives across assignments and sample globally balanced
    all_positive_pairs: List[Tuple[str, str, str]] = []  # (assignment_id, left_id, right_id)
    all_negative_pairs: List[Tuple[str, str, str]] = []

    for assignment_id, positives in assignment_to_positives.items():
        for a, b in positives:
            all_positive_pairs.append((assignment_id, a, b))
    # Sample negatives on-the-fly without generating all combinations
    # Target number equals number of positives (50/50 balance)
    target_negatives = len(all_positive_pairs)
    if target_negatives > 0:
        # Randomize assignment iteration order
        assignment_ids = list(assignment_to_id_and_path.keys())
        rng.shuffle(assignment_ids)

        # We iterate assignments and within each, iterate randomized student pairs
        for assignment_id in assignment_ids:
            if len(all_negative_pairs) >= target_negatives:
                break
            id_to_path = assignment_to_id_and_path[assignment_id]
            positives = assignment_to_positives.get(assignment_id, set())
            students = list(id_to_path.keys())
            if len(students) < 2:
                continue
            rng.shuffle(students)
            # Iterate pairs in randomized order induced by shuffled list
            for i in range(len(students)):
                if len(all_negative_pairs) >= target_negatives:
                    break
                for j in range(i + 1, len(students)):
                    if len(all_negative_pairs) >= target_negatives:
                        break
                    a_id = students[i]
                    b_id = students[j]
                    # Use sorted tuple for consistent membership check
                    key = (a_id, b_id) if a_id < b_id else (b_id, a_id)
                    if key in positives:
                        continue
                    all_negative_pairs.append((assignment_id, key[0], key[1]))

    rng.shuffle(all_positive_pairs)
    rng.shuffle(all_negative_pairs)

    # Helper cache to avoid reparsing files repeatedly
    ast_cache: Dict[Path, AST] = {}

    def get_ast(p: Path) -> AST:
        cached = ast_cache.get(p)
        if cached is not None:
            return cached
        ast = _parse_file_to_ast(p)
        ast_cache[p] = ast
        return ast

    entries: List[Tuple[Tuple[AST, AST], float, Tuple[Path, Path]]] = []

    # Build positive entries
    for assignment_id, a_id, b_id in all_positive_pairs:
        id_to_path = assignment_to_id_and_path.get(assignment_id)
        if id_to_path is None:
            continue
        left_path = id_to_path.get(a_id)
        right_path = id_to_path.get(b_id)
        if left_path is None or right_path is None:
            continue
        left_ast = get_ast(left_path)
        right_ast = get_ast(right_path)
        entries.append(((left_ast, right_ast), 1.0, (left_path, right_path)))

    # Build negative entries
    for assignment_id, a_id, b_id in all_negative_pairs:
        id_to_path = assignment_to_id_and_path.get(assignment_id)
        if id_to_path is None:
            continue
        left_path = id_to_path.get(a_id)
        right_path = id_to_path.get(b_id)
        if left_path is None or right_path is None:
            continue
        left_ast = get_ast(left_path)
        right_ast = get_ast(right_path)
        entries.append(((left_ast, right_ast), 0.0, (left_path, right_path)))

    return entries


__all__ = [
    "load_balanced_pairs",
]


