from pathlib import Path
import argparse
import sys
import traceback
from ..run import Config, MetricWeight, similarity_files


ap = argparse.ArgumentParser(description="AST-based plagiarism checker (Tree-sitter).")
ap.add_argument("left", type=Path, help="First source file")
ap.add_argument("right", type=Path, help="Second source file")
ap.add_argument("--lang", type=str, default=None, help="Force language (e.g., python, cpp, java, javascript, typescript, go, rust, c_sharp, php, ruby, swift, kotlin, scala, bash, lua)")
#ap.add_argument("--json", action="store_true", help="Output raw JSON")
args = ap.parse_args()

try:
    result = similarity_files(args.left, args.right, config=Config(
        pqgram_p=2,
        pqgram_q=2,
        pqgram_weight=MetricWeight(x_scale=1.0, exponent=2.0),
        ted_weight=MetricWeight(x_scale=1.0, exponent=1.0),
    ), lang_name=args.lang)
except Exception as e:
    print(f"Error: {e}", file=sys.stderr)
    print(f"Traceback: {traceback.format_exc()}", file=sys.stderr)
    sys.exit(2)

print(result)
