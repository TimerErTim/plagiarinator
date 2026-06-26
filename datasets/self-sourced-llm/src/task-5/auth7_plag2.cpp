#include <unordered_map>
#include <string>
#include <sstream>
#include <list>
#include <iostream>
// adapted from classmate submission

template <typename Key, typename Value>
class LRUCache {
    std::size_t limit_;
    std::list<std::pair<Key, Value>> chain_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> idx_;

public:
    LRUCache(std::size_t s) : limit_(s) {}

    bool get(const Key& k, Value& v) {
        auto f = idx_.find(k);
        if (f == idx_.end()) return false;
        v = f->second->second;
        chain_.splice(chain_.begin(), chain_, f->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        auto f = idx_.find(k);
        if (f != idx_.end()) {
            f->second->second = v;
            chain_.splice(chain_.begin(), chain_, f->second);
            return;
        }
        while (chain_.size() >= limit_) {
            idx_.erase(chain_.back().first);
            chain_.pop_back();
        }
        chain_.push_front({k, v});
        idx_[k] = chain_.begin();
    }
};

int main()
{
    std::stringstream in;
    in << std::cin.rdbuf();
    std::size_t cap;
    std::string w;
    in >> w >> cap;
    LRUCache<std::string, int> lru(cap);
    std::string ln;
    while (in >> ln)
{
        if (ln == "END") break;
        if (ln == "GET")
{
            std::string k; in >> k;
            int v;
            std::cout << (lru.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << '\n';
        } else if (ln == "PUT") {
            std::string k; int v; in >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
