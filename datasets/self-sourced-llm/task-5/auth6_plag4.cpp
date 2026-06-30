#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

template <typename Key, typename Value>
class LRUCache {
    using NodeList = std::list<std::pair<Key, Value>>;
    using MapType = std::unordered_map<Key, typename NodeList::iterator>;

    std::size_t word_;
    NodeList elem_;
    MapType ref_;

    void moveToFront(typename NodeList::iterator where) {
        elem_.splice(elem_.begin(), elem_, where);
    }

    void dropOldest() {
        ref_.erase(elem_.back().first);
        elem_.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : word_(capacity) {}

    bool get(const Key& key, Value& output) {
        auto iterator = ref_.find(key);
        if (iterator == ref_.end())
            return false;
        output = iterator->second->second;
        moveToFront(iterator->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto iterator = ref_.find(key);
        if (iterator != ref_.end()) {
            iterator->second->second = value;
            moveToFront(iterator->second);
            return;
        }
        if (elem_.size() >= word_)
            dropOldest();
        elem_.emplace_front(key, value);
        ref_[key] = elem_.begin();
    }
};

int main() {
    std::string capacityTag;
    std::size_t capacityValue;
    std::cin >> capacityTag >> capacityValue;
    LRUCache<std::string, int> ref(capacityValue);

    std::string elem;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "GET") {
            std::string key;
            std::cin >> key;
            int value;
            if (ref.get(key, value))
                std::cout << "HIT " << value << '\n';
            else
                std::cout << "MISS\n";
        } else if (elem == "PUT") {
            std::string key;
            int value;
            std::cin >> key >> value;
            ref.put(key, value);
        }
    }
    return 0;
}
