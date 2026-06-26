#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

template <typename Key, typename Value>
class LRUCache {
    std::size_t word_;
    std::list<std::pair<Key, Value>> elem_;
    std::unordered_map<Key, decltype(elem_.begin())> ref_;

public:
    LRUCache(std::size_t capacity) : word_(capacity) {}

    bool get(const Key& key, Value& out) {
        if (!ref_.count(key)) return false;
        auto it = ref_[key];
        out = it->second;
        elem_.splice(elem_.begin(), elem_, it);
        return true;
    }

    void put(const Key& key, const Value& val) {
        if (ref_.count(key)) {
            ref_[key]->second = val;
            elem_.splice(elem_.begin(), elem_, ref_[key]);
            return;
        }
        if (elem_.size() == word_) {
            ref_.erase(elem_.back().first);
            elem_.pop_back();
        }
        elem_.emplace_front(key, val);
        ref_[key] = elem_.begin();
    }
};

int main() {
    std::string capWord;
    std::size_t capVal;
    std::cin >> capWord >> capVal;
    LRUCache<std::string, int> lru(capVal);
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "GET") {
            std::string k; std::cin >> k;
            int v;
            std::cout << (lru.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << '\n';
        } else if (elem == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
