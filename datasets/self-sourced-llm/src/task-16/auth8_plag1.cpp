#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
// cleaned up version

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

int main() {
    ImageCache ic;
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "LOAD") {
            std::string name;
            int count;
            std::cin >> name >> count;
            {
                std::shared_ptr<Image> handle = ic.load(name, count);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << std::endl;
            }
        } else if (token == "QUERY") {
            std::string name;
            std::cin >> name;
            if (auto handle = ic.try_get(name))
                std::cout << "pixels=" << handle->pixel_count << std::endl;
            else
                std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
