#include <unordered_map>
#include <iostream>
#include <list>
#include <string>
// cleaned up version

template <typename Key, typename Value>
class LRUCache {
    using NodeList = std::list<std::pair<Key, Value>>;
    using MapType = std::unordered_map<Key, typename NodeList::iterator>;

    std::size_t maxItems;
    NodeList queue;
    MapType map;

    void moveToFront(typename NodeList::iterator where) {
        queue.splice(queue.begin(), queue, where);
    }

    void dropOldest() {
        map.erase(queue.back().first);
        queue.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : maxItems(capacity) {}

    bool get(const Key& key, Value& output) {
        auto iterator = map.find(key);
        if (iterator == map.end())
            return false;
        output = iterator->second->second;
        moveToFront(iterator->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto iterator = map.find(key);
        if (iterator != map.end()) {
            iterator->second->second = value;
            moveToFront(iterator->second);
            return;
        }
        if (queue.size() >= maxItems)
            dropOldest();
        queue.emplace_front(key, value);
        map[key] = queue.begin();
    }
};

int main() {
    std::string capacityTag;
    std::size_t capacityValue;
    std::cin >> capacityTag >> capacityValue;
    LRUCache<std::string, int> cache(capacityValue);

    std::string command;
    while (std::cin >> command) {
        if (command == "END") break;
        if (command == "GET") {
            std::string key;
            std::cin >> key;
            int value;
            if (cache.get(key, value))
                std::cout << "HIT " << value << std::endl;
            else
                std::cout << "MISS" << std::endl;
        } else if (command == "PUT") {
            std::string key;
            int value;
            std::cin >> key >> value;
            cache.put(key, value);
        }
    }
    return 0;
}
