#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

template <typename Key, typename Value>
class LRUCache {
    using NodeList = std::list<std::pair<Key, Value>>;
    using MapType = std::unordered_map<Key, typename NodeList::iterator>;

    std::size_t capacityLimit;
    NodeList recencyList;
    MapType keyIndex;

    void moveToFront(typename NodeList::iterator where) {
        recencyList.splice(recencyList.begin(), recencyList, where);
    }

    void dropOldest() {
        keyIndex.erase(recencyList.back().first);
        recencyList.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : capacityLimit(capacity) {}

    bool get(const Key& key, Value& output) {
        auto iterator = keyIndex.find(key);
        if (iterator == keyIndex.end())
            return false;
        output = iterator->second->second;
        moveToFront(iterator->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto iterator = keyIndex.find(key);
        if (iterator != keyIndex.end()) {
            iterator->second->second = value;
            moveToFront(iterator->second);
            return;
        }
        if (recencyList.size() >= capacityLimit)
            dropOldest();
        recencyList.emplace_front(key, value);
        keyIndex[key] = recencyList.begin();
    }
};

int main() {
    std::string capacityTag;
    std::size_t capacityValue;
    std::cin >> capacityTag >> capacityValue;
    LRUCache<std::string, int> ref(capacityValue);

    std::string command;
    while (std::cin >> command) {
        if (command == "END") break;
        if (command == "GET") {
            std::string key;
            std::cin >> key;
            int value;
            if (ref.get(key, value))
                std::cout << "HIT " << value << '\n';
            else
                std::cout << "MISS" << '\n';
        } else if (command == "PUT") {
            std::string key;
            int value;
            std::cin >> key >> value;
            ref.put(key, value);
        }
    }
    return 0;
}
