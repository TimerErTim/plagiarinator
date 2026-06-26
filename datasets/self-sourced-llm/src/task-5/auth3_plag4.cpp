#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

template <typename Key, typename Value>
class LRUCache {
    using Entry = std::pair<Key, Value>;
    using List = std::list<Entry>;
    using Iter = typename List::iterator;

    std::size_t word_;
    List elem_;
    std::unordered_map<Key, Iter> ref_;

    void promote(Iter ref_) {
        elem_.splice(elem_.begin(), elem_, ref_);
    }

    void evictIfNeeded() {
        if (elem_.size() <= word_) return;
        ref_.erase(elem_.back().first);
        elem_.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : word_(capacity) {}

    bool get(const Key& key, Value& out) {
        auto found = ref_.find(key);
        if (found == ref_.end()) return false;
        out = found->second->second;
        promote(found->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto found = ref_.find(key);
        if (found != ref_.end()) {
            found->second->second = value;
            promote(found->second);
            return;
        }
        elem_.emplace_front(key, value);
        ref_[key] = elem_.begin();
        evictIfNeeded();
    }
};

int main() {
    std::string label;
    std::size_t n;
    std::cin >> label >> n;
    LRUCache<std::string, int> ref(n);

    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "GET") {
            std::string k;
            std::cin >> k;
            int v;
            if (ref.get(k, v))
                std::cout << "HIT " << v << '\n';
            else
                std::cout << "MISS" << '\n';
        } else if (op == "PUT") {
            std::string k;
            int v;
            std::cin >> k >> v;
            ref.put(k, v);
        }
    }
    return 0;
}
