# Task 4: RAII File Writer with Move Semantics (Rule of Five)

## Summary

Implement an RAII wrapper around a buffered output file stream with full move support and use it to write a formatted report from parsed records.

## Problem

Create class `FileWriter` that opens a file in the constructor, exposes `write_line(const std::string& line)`, and closes the file in the destructor. Implement the **Rule of Five**: destructor, copy constructor, copy assignment, move constructor, move assignment.

Copying should be disabled or implemented as deep copy of file path only if file reopen succeeds; **preferred approach: disable copying, enable moving** (still declare all five explicitly with `= delete` / defined move ops).

Read records: each line `name,score` (score is integer 0–100). Write to output file `report.txt`:

```
Student Report
--------------
name: score [GRADE]
...
Average: XX.X
```

Grade mapping: A ≥ 90, B ≥ 80, C ≥ 70, D ≥ 60, else F. Print `Report written to report.txt` to stdout.

## Input

```
Alice,95
Bob,72
Carol,58
```

## Output (stdout)

```
Report written to report.txt
```

## Constraints

- C++17, single file, ~100–110 LOC.
- Must demonstrate **move semantics** and **Rule of Five** declarations.
- Use references for `write_line` parameter.

## Required C++ Concepts

- RAII
- **Rule of Five** (move ctor/assignment, deleted copies)
- **`const` references**
- File I/O (`std::ofstream` or `FILE*`)
- Class encapsulation

## Implementation Notes

- Move constructor transfers ownership and nulls source handle.
- `main` reads stdin, moves `FileWriter` into a helper if desired, writes report.
- Format average with one decimal place.
