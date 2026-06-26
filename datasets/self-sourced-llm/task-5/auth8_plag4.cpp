#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

template <typename Key, typename Value>
class LRUCache {
    struct Item { Key k; Value v; };
    std::size_t word_;
    std::list<Item> elem_;
    std::unordered_map<Key, typename std::list<Item>::iterator> where_;

    void bump(typename std::list<Item>::iterator it) {
        elem_.splice(elem_.begin(), elem_, it);
    }

public:
    explicit LRUCache(std::size_t limit) : word_(limit) {}

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
        if (elem_.size() >= word_) {
            where_.erase(elem_.back().k);
            elem_.pop_back();
        }
        elem_.push_front(Item{key, val});
        where_[key] = elem_.begin();
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
            if (lru.get(k, v)) std::cout << "HIT " << v << "\n";
            else std::cout << "MISS\n";
        } else if (op == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
