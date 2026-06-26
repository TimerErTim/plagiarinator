#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    std::size_t cap_;
    std::list<std::pair<Key, Value>> order_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> map_;

    void touch(typename std::list<std::pair<Key, Value>>::iterator it) {
        order_.splice(order_.begin(), order_, it);
    }

public:
    explicit LRUCache(std::size_t capacity) : cap_(capacity) {}

    bool get(const Key& key, Value& out) {
        auto it = map_.find(key);
        if (it == map_.end()) return false;
        out = it->second->second;
        touch(it->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            it->second->second = value;
            touch(it->second);
            return;
        }
        if (order_.size() >= cap_) {
            map_.erase(order_.back().first);
            order_.pop_back();
        }
        order_.emplace_front(key, value);
        map_[key] = order_.begin();
    }
};

int main() {
    std::size_t cap;
    std::string tag;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> cache(cap);

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "GET") {
            std::string key;
            std::cin >> key;
            int val;
            if (cache.get(key, val))
                std::cout << "HIT " << val << std::endl;
            else
                std::cout << "MISS\n";
        } else if (cmd == "PUT") {
            std::string key;
            int val;
            std::cin >> key >> val;
            cache.put(key, val);
        }
    }
    return 0;
}
