#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
// adapted from classmate submission

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> map_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::shared_ptr<Image> current;
        auto pos = map_.find(key);
        if (pos != map_.end()) current = pos->second.lock();
        if (current) return current;
        current = std::make_shared<Image>(Image{key, pixels});
        map_[key] = current;
        return current;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& entry : map_)
            if (!entry.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { map_.clear(); }

    std::shared_ptr<Image> try_get(const std::string& key) const {
        auto pos = map_.find(key);
        if (pos == map_.end()) return {};
        return pos->second.lock();
    }
};

int main()
{
    ImageCache store;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "LOAD") {
            std::string name;
            int count;
            std::cin >> name >> count;
            {
                std::shared_ptr<Image> ptr = store.load(name, count);
                std::cout << "loaded " << ptr->name << " pixels=" << ptr->pixel_count << '\n';
            }
        } else if (op == "QUERY") {
            std::string name;
            std::cin >> name;
            if (auto ptr = store.try_get(name))
                std::cout << "pixels=" << ptr->pixel_count << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            store.drop_all();
        }
    }
    return 0;
}
