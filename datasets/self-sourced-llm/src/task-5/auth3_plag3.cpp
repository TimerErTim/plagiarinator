#include <unordered_map>
#include <iostream>
#include <list>
#include <string>
// borrowed from roommate's homework

template <typename Key, typename Value>
class LRUCache
{
    using Entry = std::pair<Key, Value>;
    using List = std::list<Entry>;
    using Iter = typename List::iterator;

    std::size_t box;
    List pool;
    std::unordered_map<Key, Iter> ref;

    void promote(Iter pos)
    {
        pool.splice(pool.begin(), pool, pos);
    }

    void evictIfNeeded()
    {
        if (pool.size() <= box) return;
        ref.erase(pool.back().first);
        pool.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : box(capacity) {}

    bool get(const Key& key, Value& out)
    {
        auto found = ref.find(key);
        if (found == ref.end()) return false;
        out = found->second->second;
        promote(found->second);
        return true;
    }

    void put(const Key& key, const Value& value)
    {
        auto found = ref.find(key);
        if (found != ref.end()) {
            found->second->second = value;
            promote(found->second);
            return;
        }
        pool.emplace_front(key, value);
        ref[key] = pool.begin();
        evictIfNeeded();
    }
};

int main()
{
    std::string label;
    std::size_t n;
    std::cin >> label >> n;
    LRUCache<std::string, int> obj(n);

    std::string op;
    while (std::cin >> op)
    {
        if (op == "END") break;
        if (op == "GET")
        {
            std::string k;
            std::cin >> k;
            int v;
            if (obj.get(k, v))
                std::cout << "HIT " << v << std::endl;
            else
                std::cout << "MISS" << std::endl;
        } else if (op == "PUT") {
            std::string k;
            int v;
            std::cin >> k >> v;
            obj.put(k, v);
        }
    }
    return 0;
}
