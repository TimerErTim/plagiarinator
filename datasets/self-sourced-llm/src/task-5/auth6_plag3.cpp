#include <list>
#include <iostream>
#include <unordered_map>
#include <string>
// borrowed from roommate's homework

template <typename Key, typename Value>
class LRUCache
{
    using NodeList = std::list<std::pair<Key, Value>>;
    using MapType = std::unordered_map<Key, typename NodeList::iterator>;

    std::size_t box;
    NodeList pool;
    MapType ref;

    void moveToFront(typename NodeList::iterator where)
    {
        pool.splice(pool.begin(), pool, where);
    }

    void dropOldest()
    {
        ref.erase(pool.back().first);
        pool.pop_back();
    }

public:
    explicit LRUCache(std::size_t capacity) : box(capacity) {}

    bool get(const Key& key, Value& output)
    {
        auto iterator = ref.find(key);
        if (iterator == ref.end())
            return false;
        output = iterator->second->second;
        moveToFront(iterator->second);
        return true;
    }

    void put(const Key& key, const Value& value)
    {
        auto iterator = ref.find(key);
        if (iterator != ref.end()) {
            iterator->second->second = value;
            moveToFront(iterator->second);
            return;
        }
        if (pool.size() >= box)
            dropOldest();
        pool.emplace_front(key, value);
        ref[key] = pool.begin();
    }
};

int main()
{
    std::string capacityTag;
    std::size_t capacityValue;
    std::cin >> capacityTag >> capacityValue;
    LRUCache<std::string, int> obj(capacityValue);

    std::string command;
    while (std::cin >> command)
    {
        if (command == "END") break;
        if (command == "GET")
        {
            std::string key;
            std::cin >> key;
            int value;
            if (obj.get(key, value))
                std::cout << "HIT " << value << std::endl;
            else
                std::cout << "MISS" << std::endl;
        } else if (command == "PUT") {
            std::string key;
            int value;
            std::cin >> key >> value;
            obj.put(key, value);
        }
    }
    return 0;
}
