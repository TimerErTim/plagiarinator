#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
// adapted from classmate submission

using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::pair;
using std::size_t;
using std::string;
using std::unordered_map;

template <typename Key, typename Value>
class LRUCache
{
    size_t limit;
    list<pair<Key, Value>> chain;
    unordered_map<Key, typename list<pair<Key, Value>>::iterator> idx;

public:
    explicit LRUCache(size_t c): limit(c) {}

    bool get(const Key& k, Value& out)
    {
        auto f = idx.find(k);
        if (f == idx.end()) return false;
        out = f->second->second;
        chain.splice(chain.begin(), chain, f->second);
        return true;
    }

    void put(const Key& k, const Value& v)
    {
        auto f = idx.find(k);
        if (f != idx.end()) {
            f->second->second = v;
            chain.splice(chain.begin(), chain, f->second);
            return;
        }
        if (chain.size() >= limit) {
            idx.erase(chain.back().first);
            chain.pop_back();
        }
        chain.emplace_front(k, v);
        idx[k] = chain.begin();
    }
};

int main()
{
    string tok;
    size_t cap;
    cin >> tok >> cap;
    LRUCache<string, int> lru(cap);
    string verb;
    while (cin >> verb)
{
        if (verb == "END") break;
        if (verb == "GET")
{
            string k; cin >> k;
            int v;
            if (lru.get(k, v)) cout << "HIT " << v << endl;
            else cout << "MISS" << endl;
        } else if (verb == "PUT") {
            string k; int v; cin >> k >> v;
            lru.put(k, v);
        }
    }
    return 0;
}
