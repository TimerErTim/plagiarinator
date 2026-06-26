# Task 17: SFINAE Type-Safe Sort Utility

## Summary

Implement overloaded sort helpers: one for numeric types with ascending order, one for strings by length then lexicographic, using SFINAE / `std::enable_if`.

## Problem

Provide in namespace `util`:

```cpp
template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data);

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data);
```

For arithmetic: ascending numeric sort.

For non-arithmetic (this task: `std::string` only): sort by length ascending; ties broken lexicographically.

Read `MODE` (`NUM` or `STR`), count `n`, then `n` values. Call appropriate `sort_values` and print space-separated result.

## Example (NUM)

**Input:**
```
NUM
5
3.3 1 10 -2 5
```

**Output:**
```
-2 1 3.3 5 10
```

## Example (STR)

**Input:**
```
STR
4
bb a zzz aa
```

**Output:**
```
a aa bb zzz
```

## Constraints

- C++17, single file, ~90–110 LOC.
- **SFINAE** with `std::enable_if` (or C++17 `if constexpr` alternative documented in comments only if using enable_if in submission)
- Sort in-place via **`std::vector<T>&` reference**

## Required C++ Concepts

- **SFINAE / std::enable_if**
- **Function templates**
- **`std::is_arithmetic`**
- Reference parameters
- `std::sort` with custom comparators

## Implementation Notes

- No duplicate main logic for NUM vs STR beyond type dispatch.
- Doubles printed with default stream formatting.
