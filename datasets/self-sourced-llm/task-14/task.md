# Task 14: Dynamic Array (Vector) — Rule of Three

## Summary

Implement a simplified `Vector<T>` template with manual storage and the Rule of Three, then use it to filter and aggregate data.

## Problem

Create `template <typename T> class Vector` with:

- Default ctor, dtor, **copy ctor, copy assignment (Rule of Three)**
- `void push_back(const T& value)` and overload `void push_back(T&& value)`
- `T& operator[](std::size_t i)` + const overload
- `std::size_t size() const`
- `T* data()` / `const T* data() const`

Read count `n`, then `n` integers. Store in `Vector<int>`. Remove duplicates preserving first occurrence (use references when comparing). Print sum and product of remaining elements mod 1_000_000_007.

## Input

```
5
2 3 2 4 3
```

## Output

```
SUM 9
PROD 24
```

(Elements 2, 3, 4 → sum 9, product 24.)

## Constraints

- C++17, single file, ~100–120 LOC.
- Manual `new[]`/`delete[]` reallocation on growth
- **Rule of Three** + move optional but at least copy three
- **`const` references** for push_back const overload

## Required C++ Concepts

- **Class templates**
- **Rule of Three**
- **`const` and non-const reference** overloads for `operator[]`
- Reallocation and exception-safe assignment (basic guarantee)

## Implementation Notes

- Start capacity 4, double on overflow.
- Self-assignment safe copy assignment.
- Product mod 1e9+7; use `long long` accumulator.
