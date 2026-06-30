#include <unordered_map>
#include <iostream>
#include <list>
#include <string>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    using Entry = std::pair<Key, Value>;
    using List = std::list<Entry>;
    using Iter = typename List::iterator;

    std::size_t maxCap;
    List nodes_;
    std::unordered_map<Key, Iter> table_;

    void promote(Iter pos) {
        nodes_.splice(nodes_.begin(), nodes_, pos);
    }

    void evictIfNeeded() {
        if (nodes_.size() <= maxCap) return;
        table_.erase(nodes_.back().first);
        nodes_.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : maxCap(capacity) {}

    bool get(const Key& key, Value& out) {
        auto found = table_.find(key);
        if (found == table_.end()) return false;
        out = found->second->second;
        promote(found->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto found = table_.find(key);
        if (found != table_.end()) {
            found->second->second = value;
            promote(found->second);
            return;
        }
        nodes_.emplace_front(key, value);
        table_[key] = nodes_.begin();
        evictIfNeeded();
    }
};

int main() {
    std::string header;
    std::size_t n;
    std::cin >> header >> n;
    LRUCache<std::string, int> cache(n);

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "GET") {
            std::string k;
            std::cin >> k;
            int v;
            if (cache.get(k, v))
                std::cout << "HIT " << v << std::endl;
            else
                std::cout << "MISS" << std::endl;
        } else if (cmd == "PUT") {
            std::string k;
            int v;
            std::cin >> k >> v;
            cache.put(k, v);
        }
    }
    return 0;
}
