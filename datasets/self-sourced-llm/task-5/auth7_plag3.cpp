#include <list>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
// borrowed from roommate's homework

template <typename Key, typename Value>
class LRUCache
{
    std::size_t box;
    std::list<std::pair<Key, Value>> pool;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref;

public:
    LRUCache(std::size_t s) : box(s) {}

    bool get(const Key& k, Value& v)
    {
        auto f = ref.find(k);
        if (f == ref.end()) return false;
        v = f->second->second;
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
        while (pool.size() >= box) {
            ref.erase(pool.back().first);
            pool.pop_back();
        }
        pool.push_front({k, v});
        ref[k] = pool.begin();
    }
};

int main()
{
    std::stringstream in;
    in << std::cin.rdbuf();
    std::size_t cap;
    std::string w;
    in >> w >> cap;
    LRUCache<std::string, int> obj(cap);
    std::string line;
    while (in >> line)
    {
        if (line == "END") break;
        if (line == "GET")
        {
            std::string k; in >> k;
            int v;
            std::cout << (obj.get(k, v) ? "HIT " + std::to_string(v) : "MISS") << std::endl;
        } else if (line == "PUT") {
            std::string k; int v; in >> k >> v;
            obj.put(k, v);
        }
    }
    return 0;
}
