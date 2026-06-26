# Task 19: Rvalue Reference String Builder

## Summary

Implement a move-aware `StringBuilder` class using rvalue references, `std::move`, and append operations to assemble a document from parts.

## Problem

Create `class StringBuilder` wrapping `std::string data` with:

- `StringBuilder() = default`
- `StringBuilder& append(const std::string& s)`
- `StringBuilder& append(std::string&& s)` — move-append without copy
- `StringBuilder& append_line(const std::string& s)` — append + `\n`
- `StringBuilder(StringBuilder&& other) noexcept` — move ctor stealing buffer
- `StringBuilder& operator=(StringBuilder&& other) noexcept`
- Delete copy ctor and copy assignment
- `const std::string& str() const`
- `void clear()`

Read sections until `END`. Each section:

```
SECTION title
line1
line2
```

Blank line ends section. Build document:

```
# title
line1
line2

```

Use move when pushing temporary section bodies into builder. Print final `str()` to stdout.

## Example

**Input:**
```
SECTION Intro
Hello world

SECTION Body
Item one
Item two

END
```

**Output:**
```
# Intro
Hello world

# Body
Item one
Item two

```

## Constraints

- C++17, single file, ~100 LOC.
- **Rvalue references**, **move ctor/assignment**, **`std::move`**
- Deleted copying
- **`const std::string&`** for lvalue append

## Required C++ Concepts

- **Move semantics** (Rule of Five partial)
- **Rvalue references** (`std::string&&`)
- **`const` references**
- **`noexcept`** move operations

## Implementation Notes

- Empty section still prints `# title\n\n`.
- After move-from, source builder empty.
