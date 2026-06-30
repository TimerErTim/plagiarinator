#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <unordered_map>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    std::size_t cap;
    std::list<std::pair<Key, Value>> lst;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref_;

public:
    LRUCache(std::size_t s) : cap(s) {}

    bool get(const Key& k, Value& v) {
        auto f = ref_.find(k);
        if (f == ref_.end()) return false;
        v = f->second->second;
        lst.splice(lst.begin(), lst, f->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        auto f = ref_.find(k);
        if (f != ref_.end()) {
            f->second->second = v;
            lst.splice(lst.begin(), lst, f->second);
            return;
        }
        while (lst.size() >= cap) {
            ref_.erase(lst.back().first);
            lst.pop_back();
        }
        lst.push_front({k, v});
        ref_[k] = lst.begin();
    }
};

int main() {
    std::stringstream in;
    in << std::cin.rdbuf();
    std::size_t cap;
    std::string header;
    in >> header >> cap;
    LRUCache<std::string, int> cache(cap);
    std::string buf;
    while (in >> buf) {
        if (buf == "END") break;
        if (buf == "GET") {
            std::string k; in >> k;
            int v;
            std::cout << (cache.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << std::endl;
        } else if (buf == "PUT") {
            std::string k; int v; in >> k >> v;
            cache.put(k, v);
        }
    }
    return 0;
}
