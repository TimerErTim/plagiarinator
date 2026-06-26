# Task 15: CRTP Printable Types

## Summary

Use the Curiously Recurring Template Pattern (CRTP) to add consistent formatting to several metric types parsed from input.

## Problem

Define:

```cpp
template <typename Derived>
class Printable {
public:
  std::string to_string() const {
    return static_cast<const Derived*>(this)->format();
  }
};
```

Implement `struct Distance : Printable<Distance>` storing meters as `double`, `format()` returns `"Distance: X m"`.

Similarly `Mass : Printable<Mass>` (`"Mass: X kg"`) and `Temperature : Printable<Temperature>` (Celsius, `"Temp: X C"`).

Write function template:

```cpp
template <typename T>
void print_metric(const Printable<T>& metric);
```

which prints `to_string()` plus newline. Uses **`const Printable<T>&`** reference to base.

Read lines `TYPE value` until `END`, construct appropriate type, store in `std::vector<std::variant<Distance, Mass, Temperature>>`, then print all via visitor calling `print_metric`.

## Example

**Input:**
```
DIST 42.5
MASS 10
TEMP -3.2
END
```

**Output:**
```
Distance: 42.5 m
Mass: 10 kg
Temp: -3.2 C
```

## Constraints

- C++17, single file, ~100 LOC.
- **CRTP** required
- **`const` references** to printable base

## Required C++ Concepts

- **CRTP**
- **`static_cast` in CRTP interface**
- **`std::variant`** + visitor (secondary)
- **`const` member functions**
- Template function with constrained base reference

## Implementation Notes

- One decimal place for all numeric output.
- Invalid TYPE → skip line.
