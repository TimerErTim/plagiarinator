#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
// adapted from classmate submission

template <typename Key, typename Value>
class LRUCache {
    struct Item { Key k; Value v; };
    std::size_t limit_;
    std::list<Item> chain_;
    std::unordered_map<Key, typename std::list<Item>::iterator> idx_;

    void bump(typename std::list<Item>::iterator it) {
        chain_.splice(chain_.begin(), chain_, it);
    }

public:
    explicit LRUCache(std::size_t limit) : limit_(limit) {}

    bool get(const Key& key, Value& out) {
        auto w = idx_.find(key);
        if (w == idx_.end()) return false;
        out = w->second->v;
        bump(w->second);
        return true;
    }

    void put(const Key& key, const Value& val) {
        auto w = idx_.find(key);
        if (w != idx_.end()) {
            w->second->v = val;
            bump(w->second);
            return;
        }
        if (chain_.size() >= limit_) {
            idx_.erase(chain_.back().k);
            chain_.pop_back();
        }
        chain_.push_front(Item{key, val});
        idx_[key] = chain_.begin();
    }
};

int main()
{
    std::string tag;
    std::size_t cap;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> lru{cap};
    for (std::string verb; std::cin >> verb;)
{
        if (verb == "END") break;
        if (verb == "GET")
{
            std::string k; std::cin >> k;
            int v;
            if (lru.get(k, v)) std::cout << "HIT " << v << "\n";
            else std::cout << "MISS\n";
        } else if (verb == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
