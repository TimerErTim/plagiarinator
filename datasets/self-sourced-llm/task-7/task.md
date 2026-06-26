# Task 7: Unique Pointer Factory for Vehicles

## Summary

Implement a factory returning `std::unique_ptr<Vehicle>` for different vehicle types and simulate a fleet fuel/consumption report.

## Problem

Define abstract `Vehicle` with `virtual double fuel_needed(double distance_km) const = 0` and `virtual std::string type() const = 0`. Derive `Car` (consumption L/100km), `Truck` (L/km as double), and `Bike` (always 0).

Implement:

```cpp
std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& config);
```

Config format per vehicle after kind token:

- `CAR liters_per_100km`
- `TRUCK liters_per_km`
- `BIKE` (no extra params)

Read lines `KIND ... distance` where distance is the trip km. Print:

```
Trip i: TYPE needs X.XX liters
```

Then print total liters. Use move semantics when storing vehicles in a vector.

## Example

**Input:**
```
CAR 7.5 100
TRUCK 0.35 40
BIKE 15
```

**Output:**
```
Trip 1: CAR needs 7.50 liters
Trip 2: TRUCK needs 14.00 liters
Trip 3: BIKE needs 0.00 liters
TOTAL 21.50 liters
```

## Constraints

- C++17, single file, ~100 LOC.
- Factory must return **`std::unique_ptr`**
- Virtual destructor on base class

## Required C++ Concepts

- **`std::unique_ptr`**
- Factory function
- Virtual methods
- References to `std::istream`
- Move semantics into container

## Implementation Notes

- Invalid kind line → print `ERROR` for that line and skip.
- Two decimal places for liter amounts.
