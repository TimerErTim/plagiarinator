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
    explicit LRUCache(std::size_t c): box(c) {}

    bool get(const Key& k, Value& out)
    {
        auto f = ref.find(k);
        if (f == ref.end()) return false;
        out = f->second->second;
        pool.splice(pool.begin(), pool, f->second);
        return true;
    }

    void put(const Key& k, const Value& v)
    {
        auto f = ref.find(k);
        if (f != ref.end()) {
            f->second->second = v;
            pool.splice(pool.begin(), pool, f->second);
            return;
        }
        if (pool.size() >= box) {
            ref.erase(pool.back().first);
            pool.pop_back();
        }
        pool.emplace_front(k, v);
        ref[k] = pool.begin();
    }
};

int main()
{
    std::string token;
    std::size_t cap;
    std::cin >> token >> cap;
    LRUCache<std::string, int> obj(cap);
    std::string verb;
    while (std::cin >> verb)
    {
        if (verb == "END") break;
        if (verb == "GET")
        {
            std::string k; std::cin >> k;
            int v;
            if (obj.get(k, v)) std::cout << "HIT " << v << std::endl;
            else std::cout << "MISS" << std::endl;
        } else if (verb == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            obj.put(k, v);
        }
    }
    return 0;
}
