#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

template <typename Key, typename Value>
class LRUCache {
    std::size_t cap_;
    std::list<std::pair<Key, Value>> lst_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> mp_;

public:
    explicit LRUCache(std::size_t c) : cap_(c) {}

    bool get(const Key& k, Value& o) {
        auto p = mp_.find(k);
        if (p == mp_.end()) return false;
        o = p->second->second;
        lst_.splice(lst_.begin(), lst_, p->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        if (mp_.count(k)) {
            mp_[k]->second = v;
            lst_.splice(lst_.begin(), lst_, mp_[k]);
        } else {
            if (lst_.size() >= cap_) {
                mp_.erase(lst_.back().first);
                lst_.pop_back();
            }
            lst_.emplace_front(k, v);
            mp_[k] = lst_.begin();
        }
    }
};

int main() {
    std::string s; std::size_t n;
    std::cin >> s >> n;
    LRUCache<std::string, int> c(n);
    std::string cmd, k; int v;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "GET") {
            std::cin >> k;
            if (c.get(k, v)) std::cout << "HIT " << v << '\n';
            else std::cout << "MISS\n";
        } else if (cmd == "PUT") {
            std::cin >> k >> v;
            c.put(k, v);
        }
    }
    return 0;
}
