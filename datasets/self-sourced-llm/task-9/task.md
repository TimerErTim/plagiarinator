# Task 9: Observer Pattern with std::function

## Summary

Build a temperature sensor subject that notifies observer callbacks registered as `std::function`, and replay a scripted event log.

## Problem

Implement `class Sensor` with:

- `void subscribe(const std::string& name, std::function<void(double)> callback)`
- `void set_value(double celsius)` — updates reading and notifies all subscribers
- `double value() const`

Observers are stored in `std::vector<std::pair<std::string, std::function<void(double)>>>`. On notify, print to stdout:

```
[name] temp=<value>
```

before or after invoking callback (print first, then callback may be empty).

Read commands:

- `SUB name` — registers lambda that prints nothing extra (only dispatch line above)
- `SET value`
- `END`

## Example

**Input:**
```
SUB kitchen
SUB attic
SET 21.5
SET 19.0
END
```

**Output:**
```
[kitchen] temp=21.5
[attic] temp=21.5
[kitchen] temp=19.0
[attic] temp=19.0
```

## Constraints

- C++17, single file, ~90–110 LOC.
- Use **`std::function`** and **references** to sensor in handlers if factored.
- No global variables for sensor state.

## Required C++ Concepts

- Observer pattern
- **`std::function`**
- Lambdas and **`const` member functions**
- **`const` references** for name strings
- Encapsulation

## Implementation Notes

- Subscriptions persist for lifetime of program.
- Values printed with one decimal place.
