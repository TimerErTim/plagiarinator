# Task 11: Variadic Template Pack Logger

## Summary

Implement a variadic template `log_line` that formats heterogeneous arguments and a small config parser that uses it.

## Problem

Write:

```cpp
template <typename... Args>
void log_line(std::ostream& out, Args&&... args);
```

Each argument is printed separated by a single space, followed by newline. Support types: `int`, `double` (fixed 2 decimals), `const char*`, `std::string`.

Build `class Config` reading `key=value` lines until blank line. Store in `std::unordered_map<std::string, std::string>`. Methods:

- `bool has(const std::string& key) const`
- `const std::string& get(const std::string& key) const` — undefined if missing

After parsing, process queries until `END`:

- `GET key` → print value or `MISSING`
- `LOG args...` — uses variadic log with mixed types from parsed tokens (int if all digits optional sign, else string)

## Example

**Input:**
```
host=localhost
port=8080

GET host
LOG server port started
END
```

**Output:**
```
localhost
server 8080 started
```

## Constraints

- C++17, single file, ~100 LOC.
- **`variadic templates`** and **perfect forwarding** (`Args&&...`, `std::forward`)
- **`const` references** for map access

## Required C++ Concepts

- **Variadic templates**
- Perfect forwarding
- **`const` member functions**
- **`const std::string&`** parameters
- Fold expressions or recursive print helper

## Implementation Notes

- Blank line ends config section.
- `LOG` command: all tokens after `LOG` become arguments (type detection: try parse int, else string).
