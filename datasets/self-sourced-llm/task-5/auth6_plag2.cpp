#include <unordered_map>
#include <string>
#include <list>
#include <iostream>
// adapted from classmate submission

template <typename Key, typename Value>
class LRUCache {
    using NodeList = std::list<std::pair<Key, Value>>;
    using MapType = std::unordered_map<Key, typename NodeList::iterator>;

    std::size_t limit_;
    NodeList chain_;
    MapType idx_;

    void moveToFront(typename NodeList::iterator where) {
        chain_.splice(chain_.begin(), chain_, where);
    }

    void dropOldest() {
        idx_.erase(chain_.back().first);
        chain_.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : limit_(capacity) {}

    bool get(const Key& key, Value& output) {
        auto iterator = idx_.find(key);
        if (iterator == idx_.end())
            return false;
        output = iterator->second->second;
        moveToFront(iterator->second);
        return true;
    }

    void put(const Key& key, const Value& value) {
        auto iterator = idx_.find(key);
        if (iterator != idx_.end()) {
            iterator->second->second = value;
            moveToFront(iterator->second);
            return;
        }
        if (chain_.size() >= limit_)
            dropOldest();
        chain_.emplace_front(key, value);
        idx_[key] = chain_.begin();
    }
};

int main()
{
    std::string capacityTag;
    std::size_t capacityValue;
    std::cin >> capacityTag >> capacityValue;
    LRUCache<std::string, int> lru(capacityValue);

    std::string command;
    while (std::cin >> command)
{
        if (command == "END") break;
        if (command == "GET")
{
            std::string key;
            std::cin >> key;
            int value;
            if (lru.get(key, value))
                std::cout << "HIT " << value << std::endl;
            else
                std::cout << "MISS" << std::endl;
        } else if (command == "PUT") {
            std::string key;
            int value;
            std::cin >> key >> value;
            lru.put(key, value);
        }
    }
    return 0;
}
