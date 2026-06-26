#include <utility>
#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
// adapted from classmate submission

template <typename Key, typename Value>
class LRUCache {
    std::size_t limit_;
    std::list<std::pair<Key, Value>> chain_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> idx_;

    void touch(typename std::list<std::pair<Key, Value>>::iterator it) {
        chain_.splice(chain_.begin(), chain_, it);
    }

public:
    explicit LRUCache(std::size_t capacity) : limit_(capacity) {}

    bool get(const Key& key, Value& out) {
        auto it = idx_.find(key);
        if (it == idx_.end()) return false;
        out = it->second->second;
        touch(it->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = idx_.find(key);
        if (it != idx_.end()) {
            it->second->second = value;
            touch(it->second);
            return;
        }
        if (chain_.size() >= limit_) {
            idx_.erase(chain_.back().first);
            chain_.pop_back();
        }
        chain_.emplace_front(key, value);
        idx_[key] = chain_.begin();
    }
};

int main()
{
    std::size_t cap;
    std::string tag;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> lru(cap);

    std::string verb;
    while (std::cin >> verb)
{
        if (verb == "END") break;
        if (verb == "GET")
{
            std::string key;
            std::cin >> key;
            int val;
            if (lru.get(key, val))
                std::cout << "HIT " << val << '\n';
            else
                std::cout << "MISS\n";
        } else if (verb == "PUT") {
            std::string key;
            int val;
            std::cin >> key >> val;
            lru.put(key, val);
        }
    }
    return 0;
}
