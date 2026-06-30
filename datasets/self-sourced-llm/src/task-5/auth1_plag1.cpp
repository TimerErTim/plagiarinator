#include <utility>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    std::size_t cap_;
    std::list<std::pair<Key, Value>> orderx_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> lookup_;

    void touch(typename std::list<std::pair<Key, Value>>::iterator it) {
        orderx_.splice(orderx_.begin(), orderx_, it);
    }

public:
    explicit LRUCache(std::size_t capacity) : cap_(capacity) {}

    bool get(const Key& key, Value& out) {
        auto it = lookup_.find(key);
        if (it == lookup_.end()) return false;
        out = it->second->second;
        touch(it->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = lookup_.find(key);
        if (it != lookup_.end()) {
            it->second->second = value;
            touch(it->second);
            return;
        }
        if (orderx_.size() >= cap_) {
            lookup_.erase(orderx_.back().first);
            orderx_.pop_back();
        }
        orderx_.emplace_front(key, value);
        lookup_[key] = orderx_.begin();
    }
};

int main() {
    std::size_t cap;
    std::string tag;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> cache(cap);

    std::string cmd2;
    while (std::cin >> cmd2) {
        if (cmd2 == "END") break;
        if (cmd2 == "GET") {
            std::string key;
            std::cin >> key;
            int val;
            if (cache.get(key, val))
                std::cout << "HIT " << val << std::endl;
            else
                std::cout << "MISS\n";
        } else if (cmd2 == "PUT") {
            std::string key;
            int val;
            std::cin >> key >> val;
            cache.put(key, val);
        }
    }
    return 0;
}
