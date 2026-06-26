#include <unordered_map>
#include <iostream>
#include <list>
#include <string>
// from lab partner

template <typename Key, typename Value>
class LRUCache {
    std::size_t box;
    std::list<std::pair<Key, Value>> pool;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref;

public:
    explicit LRUCache(std::size_t c) : box(c) {}

    bool get(const Key& k, Value& o) {
        auto p = ref.find(k);
        if (p == ref.end()) return false;
        o = p->second->second;
        pool.splice(pool.begin(), pool, p->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        if (ref.count(k)) {
            ref[k]->second = v;
            pool.splice(pool.begin(), pool, ref[k]);
        } else {
            if (pool.size() >= box) {
                ref.erase(pool.back().first);
                pool.pop_back();
            }
            pool.emplace_front(k, v);
            ref[k] = pool.begin();
        }
    }
};

int main() {
    std::string s; std::size_t n;
    std::cin >> s >> n;
    LRUCache<std::string, int> c(n);
    std::string ln, k; int v;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "GET") {
            std::cin >> k;
            if (c.get(k, v)) std::cout << "HIT " << v << '\n';
            else std::cout << "MISS\n";
        } else if (ln == "PUT") {
            std::cin >> k >> v;
            c.put(k, v);
        }
    }
    return 0;
}
