#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>
// quick submit

template <typename Key, typename Value>
class LRUCache {
    std::size_t word_;
    std::list<std::pair<Key, Value>> elem_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref_;

    void touch(typename std::list<std::pair<Key, Value>>::iterator it) {
        elem_.splice(elem_.begin(), elem_, it);
    }

public:
    explicit LRUCache(std::size_t capacity) : word_(capacity) {}

    bool get(const Key& key, Value& out) {
        auto it = ref_.find(key);
        if (it == ref_.end()) return false;
        out = it->second->second;
        touch(it->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = ref_.find(key);
        if (it != ref_.end()) {
            it->second->second = value;
            touch(it->second);
            return;
        }
        if (elem_.size() >= word_) {
            ref_.erase(elem_.back().first);
            elem_.pop_back();
        }
        elem_.emplace_front(key, value);
        ref_[key] = elem_.begin();
    }
};

int main() {
    std::size_t cap;
    std::string tag;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> ref(cap);

    std::string elem;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "GET") {
            std::string key;
            std::cin >> key;
            int val;
            if (ref.get(key, val))
                std::cout << "HIT " << val << '\n';
            else
                std::cout << "MISS\n";
        } else if (elem == "PUT") {
            std::string key;
            int val;
            std::cin >> key >> val;
            ref.put(key, val);
        }
    }
    return 0;
}
