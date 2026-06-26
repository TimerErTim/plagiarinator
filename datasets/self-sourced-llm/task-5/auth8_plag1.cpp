#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    struct Item { Key k; Value v; };
    std::size_t limit_;
    std::list<Item> order_;
    std::unordered_map<Key, typename std::list<Item>::iterator> where_;

    void bump(typename std::list<Item>::iterator it) {
        order_.splice(order_.begin(), order_, it);
    }

public:
    explicit LRUCache(std::size_t limit) : limit_(limit) {}

    bool get(const Key& key, Value& out) {
        auto w = where_.find(key);
        if (w == where_.end()) return false;
        out = w->second->v;
        bump(w->second);
        return true;
    }

    void put(const Key& key, const Value& val) {
        auto w = where_.find(key);
        if (w != where_.end()) {
            w->second->v = val;
            bump(w->second);
            return;
        }
        if (order_.size() >= limit_) {
            where_.erase(order_.back().k);
            order_.pop_back();
        }
        order_.push_front(Item{key, val});
        where_[key] = order_.begin();
    }
};

int main() {
    std::string tag;
    std::size_t cap;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> lru{cap};
    for (std::string op; std::cin >> op;) {
        if (op == "END") break;
        if (op == "GET") {
            std::string k; std::cin >> k;
            int v;
            if (lru.get(k, v)) std::cout << "HIT " << v << std::endl;
            else std::cout << "MISS\n";
        } else if (op == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
