# Task 12: std::variant Expression Evaluator

## Summary

Parse and evaluate arithmetic expressions with +, -, *, / on integers using `std::variant` for AST nodes and `std::visit` for evaluation.

## Problem

Define:

```cpp
struct Number { int value; };
struct Add { /* child indices or embedded */ };
struct Sub { /* ... */ };
struct Mul { /* ... */ };
struct Div { /* ... */ };

using Expr = std::variant<Number, Add, Sub, Mul, Div>;
```

For simplicity, read **postfix** integer expressions (stack-based) from stdin, one per line, until empty line. Build variant-based AST nodes allocated in `std::vector<Expr>` with indices as children, or build tree eagerly on stack.

Evaluate each line and print result integer (integer division trunc toward zero). Division by zero → print `ERROR`.

## Example

**Input:**
```
3 4 +
10 2 /
3 4 + 2 *
```

**Output:**
```
7
5
14
```

## Constraints

- C++17, single file, ~100–120 LOC.
- Must use **`std::variant`** and **`std::visit`**
- No external parser generators

## Required C++ Concepts

- **`std::variant`**
- **`std::visit`**
- Struct types and **`const` references** where applicable
- Stack-based parsing
- Recursion or iterative eval visitor

## Implementation Notes

- Postfix tokens are integers or one of `+ - * /`.
- Evaluator returns `int` via visitor pattern on `Expr`.
