# Task 20: Template Stack with Exception-Safe Storage

## Summary

Implement a fixed-capacity stack template with strong exception safety on push, plus a bracket-matching validator using references to avoid copies.

## Problem

Create `template <typename T, std::size_t N> class FixedStack` backed by `std::array<T, N>` with:

- `bool push(const T& value)` — false if full
- `bool push(T&& value)` — move push
- `bool pop(T& out)` — false if empty; writes popped value into **`out` reference**
- `bool peek(T& out) const` — inspect top without pop
- `std::size_t size() const`

Implement `bool is_balanced(const std::string& s)` using `FixedStack<char, 128>`: brackets `()[]{}` must close in LIFO order.

Read strings until `END`, print `OK` or `FAIL` for each.

## Example

**Input:**
```
([{}])
([)]
END
```

**Output:**
```
OK
FAIL
```

## Constraints

- C++17, single file, ~100 LOC.
- **Class template** with non-type template parameter
- Output via **non-const reference** (`pop`, `peek`)
- **`const` reference** for `is_balanced` input

## Required C++ Concepts

- **Class templates** with **non-type template parameters**
- **`const` references** (input string, const push)
- **Non-const references** (out parameters)
- **`std::array`**
- Move push overload

## Implementation Notes

- Ignore non-bracket characters.
- Empty string → `OK`.
- Stack full during push → treat as `FAIL` for balance check.

## Feature Coverage Summary (Dataset)

This task set collectively exercises: Rule of Three/Five, templates (class, function, variadic, non-type), smart pointers (unique, shared, weak), virtual/CRTP polymorphism, move/rvalue refs, SFINAE, variant/visit, concurrency, observers, state pattern, and reference overloads.
