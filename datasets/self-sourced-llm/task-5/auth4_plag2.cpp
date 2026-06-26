#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
// adapted from classmate submission

template <typename Key, typename Value>
class LRUCache {
    std::size_t limit_;
    std::list<std::pair<Key, Value>> chain_;
    std::unordered_map<Key, decltype(chain_.begin())> idx_;

public:
    LRUCache(std::size_t capacity) : limit_(capacity) {}

    bool get(const Key& key, Value& out) {
        if (!idx_.count(key)) return false;
        auto it = idx_[key];
        out = it->second;
        chain_.splice(chain_.begin(), chain_, it);
        return true;
    }

    void put(const Key& key, const Value& val) {
        if (idx_.count(key)) {
            idx_[key]->second = val;
            chain_.splice(chain_.begin(), chain_, idx_[key]);
            return;
        }
        if (chain_.size() == limit_) {
            idx_.erase(chain_.back().first);
            chain_.pop_back();
        }
        chain_.emplace_front(key, val);
        idx_[key] = chain_.begin();
    }
};

int main()
{
    std::string capTag;
    std::size_t limit;
    std::cin >> capTag >> limit;
    LRUCache<std::string, int> lru(limit);
    std::string verb;
    while (std::cin >> verb)
{
        if (verb == "END") break;
        if (verb == "GET")
{
            std::string k; std::cin >> k;
            int v;
            std::cout << (lru.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << '\n';
        } else if (verb == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
