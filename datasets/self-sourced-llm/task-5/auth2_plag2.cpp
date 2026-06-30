#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
// adapted from classmate submission

template <typename Key, typename Value>
class LRUCache
{
    std::size_t maxCap;
    std::list<std::pair<Key, Value>> nodes;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> loc;

public:
    explicit LRUCache(std::size_t c): maxCap(c) {}

    bool get(const Key& k, Value& out)
    {
        auto f = loc.find(k);
        if (f == loc.end()) return false;
        out = f->second->second;
        nodes.splice(nodes.begin(), nodes, f->second);
        return true;
    }

    void put(const Key& k, const Value& v)
    {
        auto f = loc.find(k);
        if (f != loc.end()) {
            f->second->second = v;
            nodes.splice(nodes.begin(), nodes, f->second);
            return;
        }
        if (nodes.size() >= maxCap) {
            loc.erase(nodes.back().first);
            nodes.pop_back();
        }
        nodes.emplace_front(k, v);
        loc[k] = nodes.begin();
    }
};

int main()
{
    std::string token;
    std::size_t cap;
    std::cin >> token >> cap;
    LRUCache<std::string, int> lru(cap);
    std::string verb;
    while (std::cin >> verb)
    {
        if (verb == "END") break;
        if (verb == "GET")
        {
            std::string k; std::cin >> k;
            int v;
            if (lru.get(k, v)) std::cout << "HIT " << v << '\n';
            else std::cout << "MISS\n";
        } else if (verb == "PUT") {
            std::string k; int v; std::cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
