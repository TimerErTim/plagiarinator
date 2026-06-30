#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

template <typename Key, typename Value>
class LRUCache {
    std::size_t word_;
    std::list<std::pair<Key, Value>> elem_;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> ref_;

public:
    explicit LRUCache(std::size_t c) : word_(c) {}

    bool get(const Key& k, Value& o) {
        auto p = ref_.find(k);
        if (p == ref_.end()) return false;
        o = p->second->second;
        elem_.splice(elem_.begin(), elem_, p->second);
        return true;
    }

    void put(const Key& k, const Value& v) {
        if (ref_.count(k)) {
            ref_[k]->second = v;
            elem_.splice(elem_.begin(), elem_, ref_[k]);
        } else {
            if (elem_.size() >= word_) {
                ref_.erase(elem_.back().first);
                elem_.pop_back();
            }
            elem_.emplace_front(k, v);
            ref_[k] = elem_.begin();
        }
    }
};

int main() {
    std::string tag; std::size_t n;
    std::cin >> tag >> n;
    LRUCache<std::string, int> ref(n);
    std::string elem, k; int v;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "GET") {
            std::cin >> k;
            if (ref.get(k, v)) std::cout << "HIT " << v << '\n';
            else std::cout << "MISS\n";
        } else if (elem == "PUT") {
            std::cin >> k >> v;
            ref.put(k, v);
        }
    }
    return 0;
}
