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

public:
    explicit LRUCache(std::size_t c) : limit_(c) {}

    bool get(const Key& k, Value& o) {
        auto p = idx_.find(k);
        if (p == idx_.end()) return false;
        o = p->second->second;
        chain_.splice(chain_.begin(), chain_, p->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        if (idx_.count(k)) {
            idx_[k]->second = v;
            chain_.splice(chain_.begin(), chain_, idx_[k]);
        } else {
            if (chain_.size() >= limit_) {
                idx_.erase(chain_.back().first);
                chain_.pop_back();
            }
            chain_.emplace_front(k, v);
            idx_[k] = chain_.begin();
        }
    }
};

int main()
{
    std::string s; std::size_t n;
    std::cin >> s >> n;
    LRUCache<std::string, int> c(n);
    std::string verb, k; int v;
    while (std::cin >> verb)
{
        if (verb == "END") break;
        if (verb == "GET")
{
            std::cin >> k;
            if (c.get(k, v)) std::cout << "HIT " << v << '\n';
            else std::cout << "MISS\n";
        } else if (verb == "PUT") {
            std::cin >> k >> v;
            c.put(k, v);
        }
    }
    return 0;
}
