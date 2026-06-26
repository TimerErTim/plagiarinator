#include <unordered_map>
#include <iostream>
#include <list>
#include <string>
// from lab partner

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
    string word;
    size_t cap;
    cin >> word >> cap;
    LRUCache<string, int> obj(cap);
    string ln;
    while (cin >> ln) {
        if (ln == "END") break;
        if (ln == "GET") {
            string k; cin >> k;
            int v;
            if (obj.get(k, v)) cout << "HIT " << v << '\n';
            else cout << "MISS" << '\n';
        } else if (ln == "PUT") {
            string k; int v; cin >> k >> v;
            obj.put(k, v);
        }
    }
    return 0;
}
