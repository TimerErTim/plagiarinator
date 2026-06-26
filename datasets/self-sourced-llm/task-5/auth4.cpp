#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

template <typename Key, typename Value>
class LRUCache {
    std::size_t maxSize;
    std::list<std::pair<Key, Value>> dq;
    std::unordered_map<Key, decltype(dq.begin())> pos;

public:
    LRUCache(std::size_t capacity) : maxSize(capacity) {}

    bool get(const Key& key, Value& out) {
        if (!pos.count(key)) return false;
        auto it = pos[key];
        out = it->second;
        dq.splice(dq.begin(), dq, it);
        return true;
    }

    void put(const Key& key, const Value& val) {
        if (pos.count(key)) {
            pos[key]->second = val;
            dq.splice(dq.begin(), dq, pos[key]);
            return;
        }
        if (dq.size() == maxSize) {
            pos.erase(dq.back().first);
            dq.pop_back();
        }
        dq.emplace_front(key, val);
        pos[key] = dq.begin();
    }
};

int main() {
    std::string capWord;
    std::size_t capVal;
    std::cin >> capWord >> capVal;
    LRUCache<std::string, int> lru(capVal);
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "GET") {
            std::string k; std::cin >> k;
            int v;
            std::cout << (lru.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << '\n';
        } else if (token == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
