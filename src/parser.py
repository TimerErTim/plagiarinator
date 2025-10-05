from pathlib import Path
import sys
from typing import Optional
import tree_sitter
import tree_sitter_languages
from tree_sitter import Tree as AST

# --- Language inference helpers ------------------------------------------------


EXT_TO_LANG = {
    # C family
    ".c": "c",
    ".h": "c",
    ".cc": "cpp",
    ".cpp": "cpp",
    ".cxx": "cpp",
    ".hpp": "cpp",
    ".hh": "cpp",
    # Java
    ".java": "java",
    # Python
    ".py": "python",
    # JS / TS
    ".js": "javascript",
    ".mjs": "javascript",
    ".cjs": "javascript",
    ".jsx": "javascript",
    ".ts": "typescript",
    ".tsx": "tsx",  # tree_sitter_languages supports "tsx"
    # Go
    ".go": "go",
    # Rust
    ".rs": "rust",
    # C#
    ".cs": "c_sharp",
    # PHP
    ".php": "php",
    # Ruby
    ".rb": "ruby",
    # Swift
    ".swift": "swift",
    # Kotlin
    ".kt": "kotlin",
    ".kts": "kotlin",
    # Scala
    ".scala": "scala",
    # Shell
    ".sh": "bash",
    # Lua
    ".lua": "lua",
    # Others you can add as needed
}

def infer_language(path: Path) -> Optional[str]:
    return EXT_TO_LANG.get(path.suffix.lower())

# Parser

def parse_code(code: bytes, lang_name: str) -> AST:
    """
    Build a Tree-sitter parser for the given language and parse the code.
    """
    try:
        parser = tree_sitter_languages.get_parser(lang_name)  # provided by tree_sitter_languages
    except Exception:
        # Fallback: build from language object if parser helper not available
        lang = tree_sitter_languages.get_language(lang_name)
        parser = tree_sitter.Parser()
        parser.set_language(lang)
    tree = parser.parse(code)
    return tree

def build_asts(left_code: bytes, right_code: bytes, lang_name: str) -> tuple[AST, AST]:
    """
    Build ASTs for the given code.
    """
    left_ast = parse_code(left_code, lang_name)
    right_ast = parse_code(right_code, lang_name)
    return left_ast, right_ast


def load_asts(left_path: Path, right_path: Path, lang_name: str | None = None) -> tuple[AST, AST]:
    """
    Load ASTs for the given code.
    """
    if not left_path.exists() or not right_path.exists():
        raise FileNotFoundError("One or both files do not exist.")

    lang_inferred_1 = infer_language(left_path)
    lang_inferred_2 = infer_language(right_path)

    chosen_lang = lang_name or lang_inferred_1
    if chosen_lang is None:
        raise ValueError(
            f"Could not infer language from extension of '{left_path.name}'. Use --lang to specify."
        )
    if lang_inferred_2 is not None and chosen_lang != lang_inferred_2:
        print(
            f"Warning: '{right_path.name}' looks like {lang_inferred_2}, but using language '{chosen_lang}'.",
            file=sys.stderr,
        )

    left_code = left_path.read_bytes()
    right_code = right_path.read_bytes()
    return build_asts(left_code, right_code, chosen_lang)
