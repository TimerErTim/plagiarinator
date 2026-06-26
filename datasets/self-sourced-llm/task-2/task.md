# Task 2: Template Singly Linked List

## Summary

Implement a generic singly linked list template with iterator support and use it to merge two sorted integer sequences from input.

## Problem

Create a class template `LinkedList<T>` backed by nodes allocated on the heap. Support push_back, size, forward iteration, and a free function template `merge_sorted(const LinkedList<T>&, const LinkedList<T>&)` that returns a new sorted list without mutating the inputs (stable merge).

Read two whitespace-separated ascending integer sequences from stdin (sequence ends at newline; second sequence on the next line). Output the merged sequence space-separated on one line.

## Input

```
3 5 9
2 4 6 8
```

## Output

```
2 3 4 5 6 8 9
```

## Constraints

- C++17, single file.
- Target: ~100 LOC.
- `LinkedList` must be a class template; merge must be a function template.
- Iteration via nested `Iterator` class with `operator++`, `operator*`, `operator!=`.

## Required C++ Concepts

- **Class templates** and **function templates**
- Nested classes (iterator)
- References (`const LinkedList<T>&`)
- Manual node allocation/deallocation
- Iterator idioms

## Implementation Notes

- Node holds `T value` and `Node* next`.
- `LinkedList` owns head pointer; implement destructor that walks the list.
- `merge_sorted` uses two iterators; no `std::list` / `std::forward_list`.
- `main` parses integers into two lists, merges, prints with spaces.

## Edge Cases

- Either input line may be empty → treat as empty list.
- Duplicate values must appear in stable order (left list elements before right when equal).
