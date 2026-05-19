from __future__ import annotations

import argparse
import json
import os
import sys
from pathlib import Path

from decider_python import PlagiarismModel

DEFAULT_WEIGHTS = os.environ.get(
    "MODEL_WEIGHTS_PATH",
    "out/deep_learning/run_20260511_160716/checkpoints/model_20260511_160716_2300.bpk",
)


def main(argv: list[str] | None = None) -> None:
    parser = argparse.ArgumentParser(
        description="Compare two C++ files for plagiarism using the decider model."
    )
    parser.add_argument("file1", type=Path, help="First C++ source file")
    parser.add_argument("file2", type=Path, help="Second C++ source file")
    parser.add_argument(
        "--weights",
        type=Path,
        default=Path(DEFAULT_WEIGHTS),
        help="Path to model weights (.bpk)",
    )
    args = parser.parse_args(argv)

    if not args.file1.is_file() or not args.file2.is_file():
        print("Both inputs must be existing files.", file=sys.stderr)
        sys.exit(1)

    model = PlagiarismModel(str(args.weights))
    result = model.analyze(str(args.file1), str(args.file2))
    print(json.dumps(result, indent=2))


if __name__ == "__main__":
    main()
