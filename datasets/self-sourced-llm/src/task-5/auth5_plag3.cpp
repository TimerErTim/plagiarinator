#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// borrowed from roommate's homework

template <typename Key, typename Value>
class LRUCache
{
    std::size_t box;
    std::list<std::pair<Key, Value>> pool;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref;

public:
    explicit LRUCache(std::size_t c) : box(c) {}

    bool get(const Key& k, Value& o)
    {
        auto p = ref.find(k);
        if (p == ref.end()) return false;
        o = p->second->second;
        pool.splice(pool.begin(), pool, p->second);
        return true;
    }

    void put(const Key& k, const Value& v)
    {
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

int main()
{
    std::string tag; std::size_t n;
    std::cin >> tag >> n;
    LRUCache<std::string, int> obj(n);
    std::string cmd, k; int v;
    while (std::cin >> cmd)
    {
        if (cmd == "END") break;
        if (cmd == "GET")
        {
            std::cin >> k;
            if (obj.get(k, v)) std::cout << "HIT " << v << std::endl;
            else std::cout << "MISS" << std::endl;
        } else if (cmd == "PUT") {
            std::cin >> k >> v;
            obj.put(k, v);
        }
    }
    return 0;
}
