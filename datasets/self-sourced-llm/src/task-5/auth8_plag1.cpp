#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    struct Item { Key k; Value v; };
    std::size_t sizeCap;
    std::list<Item> seq;
    std::unordered_map<Key, typename std::list<Item>::iterator> map_;

    void bump(typename std::list<Item>::iterator it) {
        seq.splice(seq.begin(), seq, it);
    }

public:
    explicit LRUCache(std::size_t limit) : sizeCap(limit) {}

    bool get(const Key& key, Value& out) {
        auto w = map_.find(key);
        if (w == map_.end()) return false;
        out = w->second->v;
        bump(w->second);
        return true;
    }

    void put(const Key& key, const Value& val) {
        auto w = map_.find(key);
        if (w != map_.end()) {
            w->second->v = val;
            bump(w->second);
            return;
        }
        if (seq.size() >= sizeCap) {
            map_.erase(seq.back().k);
            seq.pop_back();
        }
        seq.push_front(Item{key, val});
        map_[key] = seq.begin();
    }
};

int main() {
    std::string tag;
    std::size_t cap;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> store{cap};
    for (std::string op; std::cin >> op;) {
        if (op == "END") break;
        if (op == "GET") {
            std::string k; std::cin >> k;
            int v;
            if (store.get(k, v)) std::cout << "HIT " << v << std::endl;
            else std::cout << "MISS\n";
        } else if (op == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            store.put(k, v);
        }
    }
    return 0;
}
