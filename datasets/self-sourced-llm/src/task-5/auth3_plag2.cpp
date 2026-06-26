#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
// adapted from classmate submission

template <typename Key, typename Value>
class LRUCache {
    using Entry = std::pair<Key, Value>;
    using List = std::list<Entry>;
    using Iter = typename List::iterator;

    std::size_t limit_;
    List chain_;
    std::unordered_map<Key, Iter> idx_;

    void promote(Iter idx_) {
        chain_.splice(chain_.begin(), chain_, idx_);
    }

    void evictIfNeeded() {
        if (chain_.size() <= limit_) return;
        idx_.erase(chain_.back().first);
        chain_.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : limit_(capacity) {}

    bool get(const Key& key, Value& out) {
        auto found = idx_.find(key);
        if (found == idx_.end()) return false;
        out = found->second->second;
        promote(found->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto found = idx_.find(key);
        if (found != idx_.end()) {
            found->second->second = value;
            promote(found->second);
            return;
        }
        chain_.emplace_front(key, value);
        idx_[key] = chain_.begin();
        evictIfNeeded();
    }
};

int main()
{
    std::string label;
    std::size_t n;
    std::cin >> label >> n;
    LRUCache<std::string, int> lru(n);

    std::string verb;
    while (std::cin >> verb)
{
        if (verb == "END") break;
        if (verb == "GET")
{
            std::string k;
            std::cin >> k;
            int v;
            if (lru.get(k, v))
                std::cout << "HIT " << v << std::endl;
            else
                std::cout << "MISS" << std::endl;
        } else if (verb == "PUT") {
            std::string k;
            int v;
            std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
