#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

template <typename Key, typename Value>
class LRUCache {
    using Entry = std::pair<Key, Value>;
    using List = std::list<Entry>;
    using Iter = typename List::iterator;

    std::size_t capacity_;
    List items_;
    std::unordered_map<Key, Iter> lookup_;

    void promote(Iter pos) {
        items_.splice(items_.begin(), items_, pos);
    }

    void evictIfNeeded() {
        if (items_.size() <= capacity_) return;
        lookup_.erase(items_.back().first);
        items_.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : capacity_(capacity) {}

    bool get(const Key& key, Value& out) {
        auto found = lookup_.find(key);
        if (found == lookup_.end()) return false;
        out = found->second->second;
        promote(found->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto found = lookup_.find(key);
        if (found != lookup_.end()) {
            found->second->second = value;
            promote(found->second);
            return;
        }
        items_.emplace_front(key, value);
        lookup_[key] = items_.begin();
        evictIfNeeded();
    }
};

int main() {
    std::string label;
    std::size_t n;
    std::cin >> label >> n;
    LRUCache<std::string, int> cache(n);

    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "GET") {
            std::string k;
            std::cin >> k;
            int v;
            if (cache.get(k, v))
                std::cout << "HIT " << v << std::endl;
            else
                std::cout << "MISS" << std::endl;
        } else if (op == "PUT") {
            std::string k;
            int v;
            std::cin >> k >> v;
            cache.put(k, v);
        }
    }
    return 0;
}
