# Task 6: Polymorphic Shape Hierarchy

## Summary

Use abstract base classes, virtual functions, and references to compute total area and perimeter of mixed shapes read from input.

## Problem

Define abstract base `Shape` with pure virtual `double area() const` and `double perimeter() const`, plus `virtual std::string name() const = 0`. Derive `Circle`, `Rectangle`, and `Triangle` (equilateral, given side length).

Read shape descriptions until `DONE`:

- `CIRCLE radius`
- `RECT width height`
- `TRI side`

Store shapes in `std::vector<std::unique_ptr<Shape>>`. After input, print for each shape (in order):

```
<name>: area=..., perimeter=...
```

Then print totals:

```
TOTAL area=...
TOTAL perimeter=...
```

Use **`const Shape&`** in helper functions `print_shape(const Shape&)` and `accumulate(const Shape&, double& area_sum, double& peri_sum)`.

## Example

**Input:**
```
CIRCLE 2
RECT 3 4
DONE
```

**Output:**
```
Circle: area=12.5664, perimeter=12.5664
Rectangle: area=12, perimeter=14
TOTAL area=24.5664
TOTAL perimeter=26.5664
```

## Constraints

- C++17, single file, ~100 LOC.
- **`std::unique_ptr`** for ownership.
- Virtual dispatch required; no enums replacing polymorphism.

## Required C++ Concepts

- Abstract base class / pure virtual functions
- **`const` member functions**
- **`const` references** as parameters
- **`std::unique_ptr`**
- Virtual destructor on `Shape`

## Implementation Notes

- π may be `3.141592653589793` or `M_PI`.
- Format doubles with 4 decimal places fixed.
