#include <unordered_map>
#include <iostream>
#include <list>
#include <string>
// from lab partner

template <typename Key, typename Value>
class LRUCache {
    struct Item { Key k; Value v; };
    std::size_t box;
    std::list<Item> pool;
    std::unordered_map<Key, typename std::list<Item>::iterator> ref;

    void bump(typename std::list<Item>::iterator it) {
        pool.splice(pool.begin(), pool, it);
    }

public:
    explicit LRUCache(std::size_t limit) : box(limit) {}

    bool get(const Key& key, Value& out) {
        auto w = ref.find(key);
        if (w == ref.end()) return false;
        out = w->second->v;
        bump(w->second);
        return true;
    }

    void put(const Key& key, const Value& val) {
        auto w = ref.find(key);
        if (w != ref.end()) {
            w->second->v = val;
            bump(w->second);
            return;
        }
        if (pool.size() >= box) {
            ref.erase(pool.back().k);
            pool.pop_back();
        }
        pool.push_front(Item{key, val});
        ref[key] = pool.begin();
    }
};

int main() {
    std::string tag;
    std::size_t cap;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> obj{cap};
    for (std::string op; std::cin >> op;) {
        if (op == "END") break;
        if (op == "GET") {
            std::string k; std::cin >> k;
            int v;
            if (obj.get(k, v)) std::cout << "HIT " << v << "\n";
            else std::cout << "MISS\n";
        } else if (op == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            obj.put(k, v);
        }
    }
    return 0;
}
