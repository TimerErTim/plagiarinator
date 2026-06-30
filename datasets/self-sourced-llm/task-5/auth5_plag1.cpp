#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    std::size_t cap_;
    std::list<std::pair<Key, Value>> items_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> table_;

public:
    explicit LRUCache(std::size_t c) : cap_(c) {}

    bool get(const Key& k, Value& o) {
        auto p = table_.find(k);
        if (p == table_.end()) return false;
        o = p->second->second;
        items_.splice(items_.begin(), items_, p->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        if (table_.count(k)) {
            table_[k]->second = v;
            items_.splice(items_.begin(), items_, table_[k]);
        } else {
            if (items_.size() >= cap_) {
                table_.erase(items_.back().first);
                items_.pop_back();
            }
            items_.emplace_front(k, v);
            table_[k] = items_.begin();
        }
    }
};

int main() {
    std::string tag; std::size_t n;
    std::cin >> tag >> n;
    LRUCache<std::string, int> store(n);
    std::string verb, k; int v;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "GET") {
            std::cin >> k;
            if (store.get(k, v)) std::cout << "HIT " << v << std::endl;
            else std::cout << "MISS\n";
        } else if (verb == "PUT") {
            std::cin >> k >> v;
            store.put(k, v);
        }
    }
    return 0;
}
