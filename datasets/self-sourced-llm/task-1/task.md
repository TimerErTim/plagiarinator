# Task 1: Copyable String Buffer (Rule of Three)

## Summary

Implement a heap-allocated, null-terminated string wrapper class and use it to normalize and sort a batch of student names read from standard input.

## Problem

Build a single-file C++ program centered on a `StringBuffer` class that owns a dynamically allocated `char` array. The class must manage its own memory and behave correctly when copied. Read several names (one per line), trim leading/trailing whitespace in place, discard empty lines, sort the remaining names lexicographically, and print them one per line.

Do not use `std::string`. Use raw `new[]` / `delete[]` and implement the **Rule of Three** explicitly: destructor, copy constructor, and copy assignment operator.

## Input

- Lines of text from `stdin`, terminated by EOF.
- Each line may contain arbitrary printable ASCII and spaces.
- At most 200 lines; each line has at most 80 characters before trimming.

## Output

- Print each non-empty trimmed name on its own line, sorted ascending.
- No trailing spaces on output lines.

## Constraints

- C++17 or later.
- Single translation unit (`main.cpp`).
- Target solution size: roughly 90–120 lines of code (excluding blank lines and comments).
- No external libraries beyond the C++ standard library containers allowed for sorting (e.g. `std::vector<StringBuffer>` is fine).

## Required C++ Concepts

- Classes and encapsulation
- Dynamic memory (`new[]` / `delete[]`)
- **Rule of Three** (destructor, copy constructor, copy assignment)
- `const` member functions and `const` references
- Manual in-place trimming via references to `StringBuffer`

## Implementation Notes

- Provide `StringBuffer(const char*)`, `~StringBuffer()`, copy ctor, copy assignment.
- Implement `trim()`, `c_str()`, `size()`, and `operator<` for sorting.
- Guard against self-assignment in copy assignment.
- `main` reads lines into `std::vector<StringBuffer>`, trims, erases empty entries, sorts, prints.

## Example

**Input:**
```
  alice  
bob   
  bob
charlie
```

**Output:**
```
alice
bob
bob
charlie
```
