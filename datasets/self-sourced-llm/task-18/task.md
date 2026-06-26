# Task 18: Finite State Machine with unique_ptr States

## Summary

Model a text protocol parser as a finite state machine where each state is a class owned via `std::unique_ptr` and transitions transfer ownership.

## Problem

Parse log lines of form `[LEVEL] message` where LEVEL is `INFO`, `WARN`, or `ERROR`. Invalid lines go to a `Rejected` tally.

Implement state pattern:

- Abstract `State` with `virtual std::unique_ptr<State> handle(class Parser& ctx, const std::string& line) = 0`
- States: `HeaderState`, `BodyState`, `DoneState`
- `Parser` holds `std::unique_ptr<State> state`, counters for each level, and `rejected`

Simplified flow: start in `HeaderState`. If line matches `[INFO|WARN|ERROR] rest`, increment counter and stay; else increment `rejected`. Command `END` transitions to `DoneState`.

Print summary:

```
INFO: n
WARN: n
ERROR: n
REJECTED: n
```

## Example

**Input:**
```
[INFO] started
bad line
[WARN] low disk
[ERROR] fail
END
```

**Output:**
```
INFO: 1
WARN: 1
ERROR: 1
REJECTED: 1
```

## Constraints

- C++17, single file, ~100–120 LOC.
- States transferred via **`std::unique_ptr<State>`** return values
- Virtual destructor on `State`
- **`const std::string&`** for line parameter

## Required C++ Concepts

- State pattern
- **`std::unique_ptr`**
- Virtual methods
- **`const` references**
- String parsing (`find`, `substr`)

## Implementation Notes

- Line format: `[` + LEVEL + `]` + optional space + message.
- Case-sensitive LEVEL tokens.
