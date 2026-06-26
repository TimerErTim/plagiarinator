# Task 3: Shared Ownership Graph and Reachability

## Summary

Model a directed graph with `std::shared_ptr` nodes and determine whether a path exists from a source label to a destination label.

## Problem

Define `struct GraphNode` with a `std::string label` and `std::vector<std::shared_ptr<GraphNode>> edges`. Implement `Graph` that stores nodes in a `std::unordered_map<std::string, std::shared_ptr<GraphNode>>` and supports adding directed edges.

Read commands from stdin:

- `ADD_NODE name`
- `ADD_EDGE from to`
- `QUERY from to` → print `REACHABLE` or `UNREACHABLE`
- `END` → terminate

Use shared pointers so multiple edges can reference the same node. Detect reachability with DFS or BFS from `from` to `to`.

## Example

**Input:**
```
ADD_NODE A
ADD_NODE B
ADD_NODE C
ADD_EDGE A B
ADD_EDGE B C
QUERY A C
QUERY C A
END
```

**Output:**
```
REACHABLE
UNREACHABLE
```

## Constraints

- C++17, single file, ~100 LOC.
- Must use **`std::shared_ptr`** for node ownership.
- Graph class encapsulates the map; expose methods, not raw globals.

## Required C++ Concepts

- **`std::shared_ptr`**
- Classes, methods, `const` references for string labels
- STL containers (`unordered_map`, `vector`)
- Graph traversal

## Implementation Notes

- Creating a node twice with the same name must not duplicate entries.
- `ADD_EDGE` may reference nodes not yet created → auto-create missing nodes.
- Avoid cycles causing infinite loops during search (track visited set).
