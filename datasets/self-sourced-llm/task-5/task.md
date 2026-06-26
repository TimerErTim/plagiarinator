# Task 5: Template LRU Cache

## Summary

Implement a fixed-capacity Least-Recently-Used cache as a class template and answer lookup/update queries from stdin.

## Problem

Create `template <typename Key, typename Value> class LRUCache` with:

- `LRUCache(std::size_t capacity)`
- `bool get(const Key& key, Value& out)` — returns true if found; updates recency
- `void put(const Key& key, const Value& value)` — insert or update; evict LRU when over capacity

Use a doubly-linked list order + hash map from key to list iterator (or index). Keys and values for this task are `std::string` and `int`.

Process commands until `END`:

- `GET key` → print `HIT value` or `MISS`
- `PUT key value` → no output
- `END`

## Example

**Input:**
```
PUT a 1
PUT b 2
GET a
PUT c 3
GET b
GET c
END
```

(capacity = 2, given as first line `CAPACITY 2`)

**Output:**
```
HIT 1
MISS
HIT 3
```

## Constraints

- C++17, single file, ~100–120 LOC.
- **`class template`** required.
- Use references for get/put parameters.

## Required C++ Concepts

- **Class templates**
- STL (`list`, `unordered_map`)
- **`const` references**
- Iterator invalidation awareness

## Implementation Notes

- First line of input: `CAPACITY n`.
- Evict least recently used entry on overflow.
- `get` promotes key to most recent.
