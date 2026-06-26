#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>

template <typename Key, typename Value>
class LRUCache {
    std::size_t sz_;
    std::list<std::pair<Key, Value>> q_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref_;

public:
    LRUCache(std::size_t s) : sz_(s) {}

    bool get(const Key& k, Value& v) {
        auto f = ref_.find(k);
        if (f == ref_.end()) return false;
        v = f->second->second;
        q_.splice(q_.begin(), q_, f->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        auto f = ref_.find(k);
        if (f != ref_.end()) {
            f->second->second = v;
            q_.splice(q_.begin(), q_, f->second);
            return;
        }
        while (q_.size() >= sz_) {
            ref_.erase(q_.back().first);
            q_.pop_back();
        }
        q_.push_front({k, v});
        ref_[k] = q_.begin();
    }
};

int main() {
    std::stringstream in;
    in << std::cin.rdbuf();
    std::size_t cap;
    std::string w;
    in >> w >> cap;
    LRUCache<std::string, int> cache(cap);
    std::string line;
    while (in >> line) {
        if (line == "END") break;
        if (line == "GET") {
            std::string k; in >> k;
            int v;
            std::cout << (cache.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << '\n';
        } else if (line == "PUT") {
            std::string k; int v; in >> k >> v;
            cache.put(k, v);
        }
    }
    return 0;
}
