# Task 13: Thread-Safe Bounded Queue

## Summary

Implement a fixed-capacity thread-safe queue with mutex and condition variables, then simulate producer/consumer counts from a script (single-process, threads).

## Problem

Create `template <typename T> class BlockingQueue` with:

- `BlockingQueue(std::size_t capacity)`
- `void push(T value)` — blocks while full
- `T pop()` — blocks while empty
- `std::size_t size() const`

Use `std::mutex`, `std::condition_variable`, and `std::queue<T>` internally.

Read script:

```
PRODUCERS p
CONSUMERS c
ITEMS n
```

Launch `p` producer threads each pushing `n/p` items (distribute remainder to first threads), and `c` consumer threads popping until all `n` items consumed. Each item is an integer id 0..n-1. Consumers print:

```
got id
```

Print lines may interleave. After join, print `DONE`.

## Example

**Input:**
```
PRODUCERS 2
CONSUMERS 2
ITEMS 4
```

**Output (order interleaved):**
```
got 0
got 1
got 2
got 3
DONE
```

## Constraints

- C++17, single file, ~100–120 LOC.
- **`std::mutex`**, **`std::condition_variable`**, **`std::thread`**
- Class template for queue

## Required C++ Concepts

- **Concurrency** (threads, mutex, condition_variable)
- **Class templates**
- Move semantics for `push(T value)`
- RAII lock guards

## Implementation Notes

- Join all threads before printing `DONE`.
- No busy-waiting; use condition variables.
- Main coordinates total item count with atomic or join pattern.
