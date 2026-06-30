#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    std::size_t limit;
    std::list<std::pair<Key, Value>> list_;
    std::unordered_map<Key, decltype(list_.begin())> idx;

public:
    LRUCache(std::size_t capacity) : limit(capacity) {}

    bool get(const Key& key, Value& out) {
        if (!idx.count(key)) return false;
        auto it = idx[key];
        out = it->second;
        list_.splice(list_.begin(), list_, it);
        return true;
    }

    void put(const Key& key, const Value& val) {
        if (idx.count(key)) {
            idx[key]->second = val;
            list_.splice(list_.begin(), list_, idx[key]);
            return;
        }
        if (list_.size() == limit) {
            idx.erase(list_.back().first);
            list_.pop_back();
        }
        list_.emplace_front(key, val);
        idx[key] = list_.begin();
    }
};

int main() {
    std::string capWord;
    std::size_t capVal;
    std::cin >> capWord >> capVal;
    LRUCache<std::string, int> lru(capVal);
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "GET") {
            std::string k; std::cin >> k;
            int v;
            std::cout << (lru.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << std::endl;
        } else if (word == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
