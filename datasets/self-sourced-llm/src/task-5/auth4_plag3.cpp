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
    std::unordered_map<Key, decltype(pool.begin())> ref;

public:
    LRUCache(std::size_t capacity) : box(capacity) {}

    bool get(const Key& key, Value& out)
    {
        if (!ref.count(key)) return false;
        auto it = ref[key];
        out = it->second;
        pool.splice(pool.begin(), pool, it);
        return true;
    }

    void put(const Key& key, const Value& val)
    {
        if (ref.count(key)) {
            ref[key]->second = val;
            pool.splice(pool.begin(), pool, ref[key]);
            return;
        }
        if (pool.size() == box) {
            ref.erase(pool.back().first);
            pool.pop_back();
        }
        pool.emplace_front(key, val);
        ref[key] = pool.begin();
    }
};

int main()
{
    std::string capWord;
    std::size_t capVal;
    std::cin >> capWord >> capVal;
    LRUCache<std::string, int> obj(capVal);
    std::string token;
    while (std::cin >> token)
    {
        if (token == "END") break;
        if (token == "GET")
        {
            std::string k; std::cin >> k;
            int v;
            std::cout << (obj.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << std::endl;
        } else if (token == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            obj.put(k, v);
        }
    }
    return 0;
}
