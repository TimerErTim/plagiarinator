# Task 16: Weak Pointer Image Cache

## Summary

Build a resource cache using `std::weak_ptr` to avoid reloading images, with `std::shared_ptr` ownership shared by callers.

## Problem

Implement `class ImageCache` mapping string keys to weak pointers of loaded `Image` objects. `Image` holds `std::string name` and `int pixel_count`.

Methods:

- `std::shared_ptr<Image> load(const std::string& key, int pixels)` — if weak entry expired or missing, create new `Image`, store `weak_ptr`, return `shared_ptr`; else lock and return existing
- `std::size_t size() const` — count entries with non-expired weak_ptr (lazy prune on access optional)

Read commands:

- `LOAD key pixels`
- `QUERY key` → print `pixels=N` if alive, else `EXPIRED`
- `DROP_ALL` — clear map (simulate cache flush; outstanding shared_ptrs keep objects alive)
- `END`

After each `LOAD`, print `loaded key pixels=N`.

## Example

**Input:**
```
LOAD cat 1000
LOAD cat 1000
QUERY cat
DROP_ALL
QUERY cat
END
```

**Output:**
```
loaded cat pixels=1000
loaded cat pixels=1000
pixels=1000
EXPIRED
```

(Second `LOAD` hits cache. After `DROP_ALL`, the map entry is removed and no outstanding `shared_ptr` remains, so `QUERY` reports `EXPIRED`.)

## Constraints

- C++17, single file, ~100 LOC.
- **`std::shared_ptr`**, **`std::weak_ptr`**
- **`const std::string&`** parameters

## Required C++ Concepts

- **`std::weak_ptr` / `std::shared_ptr`**
- Cache design
- **`const` member functions**
- **`const` references**

## Implementation Notes

- `main` does not retain `shared_ptr` between commands except during LOAD print line.
- Use `weak_ptr::lock()` to test liveness.
