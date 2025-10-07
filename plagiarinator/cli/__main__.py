from pathlib import Path
import argparse
import sys
import json
import traceback
import time
from typing import Dict, List, Tuple, Any, Optional
from ..run import Config, MetricWeight, similarity_files, similarity_trees
from ..parser import load_asts


class ASTCache:
    """Cache for parsed ASTs to avoid recomputation for the same files."""
    def __init__(self):
        self._cache: Dict[str, Any] = {}
    
    def get_cache_key(self, file_path: Path) -> str:
        """Generate a cache key for a file path."""
        return str(file_path.resolve())
    
    def has_ast(self, file_path: Path) -> bool:
        """Check if AST is cached for the given file."""
        return self.get_cache_key(file_path) in self._cache
    
    def get_ast(self, file_path: Path) -> Any:
        """Get cached AST for the given file."""
        return self._cache.get(self.get_cache_key(file_path))
    
    def set_ast(self, file_path: Path, ast: Any) -> None:
        """Cache AST for the given file."""
        self._cache[self.get_cache_key(file_path)] = ast


def load_config_from_json(config_path: Path) -> Config:
    """Load configuration from JSON file."""
    try:
        with open(config_path, 'r') as f:
            config_data = json.load(f)
        
        # Create MetricWeight objects from dictionaries
        def create_metric_weight(data: dict) -> MetricWeight:
            return MetricWeight(
                x_scale=data.get('x_scale', 1.0),
                exponent=data.get('exponent', 1.0),
                weight=data.get('weight', 1.0)
            )
        
        return Config(
            pqgram_p=config_data.get('pqgram_p', 2),
            pqgram_q=config_data.get('pqgram_q', 2),
            wl_subtree_jaccard_rounds=config_data.get('wl_subtree_jaccard_rounds', 2),
            pqgram_weight=create_metric_weight(config_data.get('pqgram_weight', {})),
            ted_weight=create_metric_weight(config_data.get('ted_weight', {})),
            node_cosine_weight=create_metric_weight(config_data.get('node_cosine_weight', {})),
            bigram_cosine_weight=create_metric_weight(config_data.get('bigram_cosine_weight', {})),
            wl_subtree_jaccard_weight=create_metric_weight(config_data.get('wl_subtree_jaccard_weight', {})),
        )
    except Exception as e:
        raise ValueError(f"Failed to load config from {config_path}: {e}")


def get_default_config() -> Config:
    """Get default configuration."""
    return Config(
        pqgram_p=2,
        pqgram_q=2,
        wl_subtree_jaccard_rounds=2,
        pqgram_weight=MetricWeight(x_scale=1.0, exponent=2.0, weight=1.0),
        ted_weight=MetricWeight(x_scale=1.0, exponent=1.0, weight=1.0),
        node_cosine_weight=MetricWeight(x_scale=1.0, exponent=1.0, weight=1.0),
        bigram_cosine_weight=MetricWeight(x_scale=1.0, exponent=1.0, weight=1.0),
        wl_subtree_jaccard_weight=MetricWeight(x_scale=1.0, exponent=1.0, weight=1.0),
    )


def parse_file_pairs(pairs_args: List[str]) -> List[Tuple[Path, Path]]:
    """Parse file pairs from command line arguments."""
    if len(pairs_args) % 2 != 0:
        raise ValueError("File pairs must be provided in pairs (even number of arguments)")
    
    pairs = []
    for i in range(0, len(pairs_args), 2):
        left = Path(pairs_args[i])
        right = Path(pairs_args[i + 1])
        
        if not left.exists():
            raise ValueError(f"Left file does not exist: {left}")
        if not right.exists():
            raise ValueError(f"Right file does not exist: {right}")
        
        pairs.append((left, right))
    
    return pairs


def get_cached_ast(file_path: Path, ast_cache: ASTCache, lang_name: Optional[str]) -> Any:
    """Get AST from cache or load and cache it."""
    abs_file_path = file_path.resolve()
    if ast_cache.has_ast(abs_file_path):
        return ast_cache.get_ast(abs_file_path)
    
    # Load AST and cache it
    left_ast, _ = load_asts(abs_file_path, abs_file_path, lang_name=lang_name)
    ast_cache.set_ast(abs_file_path, left_ast)
    return left_ast


def compare_file_pair(left: Path, right: Path, config: Config, lang_name: Optional[str], 
                     ast_cache: ASTCache) -> Dict[str, Any]:
    """Compare a single file pair and return detailed results."""
    try:
        # Get ASTs from cache or load them
        left_ast = get_cached_ast(left, ast_cache, lang_name)
        right_ast = get_cached_ast(right, ast_cache, lang_name)
        
        start_time = time.time()
        result = similarity_trees(left_ast, right_ast, config)
        duration = time.time() - start_time
        
        return {
            "success": True,
            "duration": f"{duration}s",
            "left_file": str(left),
            "right_file": str(right),
            "similarity": {
                "combined": result.combined,
                "results": [
                    {
                        "name": metric.name,
                        "value": metric.value
                    }
                    for metric in result.results
                ]
            },
            "error": None
        }
    except Exception as e:
        return {
            "success": False,
            "duration": None,
            "left_file": str(left),
            "right_file": str(right),
            "similarity": None,
            "error": {
                "message": str(e),
                "type": type(e).__name__,
                "traceback": traceback.format_exc()
            }
        }


def main():
    """Main CLI function."""
    ap = argparse.ArgumentParser(description="AST-based plagiarism checker (Tree-sitter).")
    ap.add_argument("file_pairs", nargs="+", help="File pairs to compare (provide pairs of files)")
    ap.add_argument("--config", type=Path, help="Path to JSON configuration file")
    ap.add_argument("--lang", type=str, default=None, 
                   help="Force language (e.g., python, cpp, java, javascript, typescript, go, rust, c_sharp, php, ruby, swift, kotlin, scala, bash, lua)")
    
    try:
        args = ap.parse_args()
        
        # Load configuration
        if args.config:
            if not args.config.exists():
                raise ValueError(f"Config file does not exist: {args.config}")
            config = load_config_from_json(args.config)
        else:
            config = get_default_config()
        
        # Parse file pairs
        file_pairs = parse_file_pairs(args.file_pairs)
        
        # Initialize AST cache
        ast_cache = ASTCache()
        
        # Process all file pairs
        results = []
        for left, right in file_pairs:
            result = compare_file_pair(left, right, config, args.lang, ast_cache)
            results.append(result)
        
        # Output results as JSON
        output = {
            "total_comparisons": len(results),
            "failed_comparisons": sum(1 for r in results if not r["success"]),
            "results": results
        }
        
        print(json.dumps(output, indent=2))
        
        # Exit with error code if any comparisons failed
        if output["failed_comparisons"] > 0:
            sys.exit(1)
            
    except Exception as e:
        # Handle top-level errors
        error_output = {
            "total_comparisons": 0,
            "successful_comparisons": 0,
            "failed_comparisons": 0,
            "results": [],
            "fatal_error": {
                "message": str(e),
                "type": type(e).__name__,
                "traceback": traceback.format_exc()
            }
        }
        print(json.dumps(error_output, indent=2), file=sys.stderr)
        sys.exit(2)


if __name__ == "__main__":
    main()
