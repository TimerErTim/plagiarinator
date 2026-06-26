# Task 10: Binary Search Tree with Reference Overloads

## Summary

Implement a BST for integers with insert/contains using const and non-const reference overloads, and print an in-order traversal.

## Problem

Create `class IntBST` with:

- `void insert(int value)`
- `bool contains(int value) const`
- `void inorder(std::vector<int>& out) const` — appends sorted values to `out`
- `int& min()` and `const int& min() const` — references to smallest element; throw or assert if empty

Read integers until `DONE`. Insert all, then process commands until second `DONE`:

- `CONTAINS x` → `YES` / `NO`
- `MIN` → print minimum
- `PRINT` → print in-order sequence space-separated

Use **`const` correctness**: `contains` and `inorder` are const; provide both mutable and const overloads for accessors that expose references to nested data.

## Example

**Input:**
```
5 2 8 2 1
DONE
CONTAINS 8
MIN
PRINT
DONE
```

**Output:**
```
YES
1
1 2 5 8
```

## Constraints

- C++17, single file, ~100 LOC.
- Manual tree nodes with raw pointers; define destructor to free tree.
- **`const` and non-const reference** overloads required on `min()`.

## Required C++ Concepts

- BST algorithms
- **`const` member functions**
- **Reference overloads** (const vs non-const)
- Recursive private helpers taking **`const IntBST&`** or node pointers
- Manual memory management

## Implementation Notes

- Duplicate inserts ignored (set semantics).
- Empty tree MIN → print `EMPTY`.
