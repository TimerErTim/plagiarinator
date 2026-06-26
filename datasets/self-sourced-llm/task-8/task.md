# Task 8: Template Matrix with Operator Overloading

## Summary

Implement a dense numeric matrix class template with arithmetic operators and use it to solve a small linear algebra task from stdin.

## Problem

Create `template <typename T> class Matrix` storing `rows × cols` data in `std::vector<T>`. Support:

- Construction from dimensions (zero-filled)
- `T& operator()(std::size_t r, std::size_t c)` and const overload
- `Matrix operator+(const Matrix& other) const`
- `Matrix operator*(const Matrix& other) const` (standard matrix multiply)
- `friend std::ostream& operator<<(std::ostream&, const Matrix&)` row-major, space-separated rows on separate lines

Read dimensions and data for matrix A, then B. Compute `C = A * B + A` (watch dimension compatibility). Print C.

## Input

```
2 2
1 2
3 4
2 2
5 6
7 8
```

## Output

```
20 24
46 54
```

(Explanation: A×B = [[19,22],[43,50]]; adding A yields [[20,24],[46,54]].)

## Constraints

- C++17, single file, ~100–120 LOC.
- Must be a **class template**
- Operator overloads return by value; take **`const Matrix&`**

## Required C++ Concepts

- **Class templates**
- **Operator overloading**
- **`const` member functions** and **`const` references**
- Bounds checking optional but index via `operator()`

## Implementation Notes

- Throw or print `ERROR` if multiply dimensions mismatch; spec: print `ERROR` to stdout and exit 0.
- `main` parses input, computes expression, prints result.
