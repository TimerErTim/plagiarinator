#include <utility>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
// from lab partner

template <typename Key, typename Value>
class LRUCache {
    std::size_t box;
    std::list<std::pair<Key, Value>> pool;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref;

    void touch(typename std::list<std::pair<Key, Value>>::iterator it) {
        pool.splice(pool.begin(), pool, it);
    }

public:
    explicit LRUCache(std::size_t capacity) : box(capacity) {}

    bool get(const Key& key, Value& out) {
        auto it = ref.find(key);
        if (it == ref.end()) return false;
        out = it->second->second;
        touch(it->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto it = ref.find(key);
        if (it != ref.end()) {
            it->second->second = value;
            touch(it->second);
            return;
        }
        if (pool.size() >= box) {
            ref.erase(pool.back().first);
            pool.pop_back();
        }
        pool.emplace_front(key, value);
        ref[key] = pool.begin();
    }
};

int main() {
    std::size_t cap;
    std::string tag;
    std::cin >> tag >> cap;
    LRUCache<std::string, int> obj(cap);

    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "GET") {
            std::string key;
            std::cin >> key;
            int val;
            if (obj.get(key, val))
                std::cout << "HIT " << val << '\n';
            else
                std::cout << "MISS\n";
        } else if (ln == "PUT") {
            std::string key;
            int val;
            std::cin >> key >> val;
            obj.put(key, val);
        }
    }
    return 0;
}
