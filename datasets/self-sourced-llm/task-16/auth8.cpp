#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> registry_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::shared_ptr<Image> current;
        auto pos = registry_.find(key);
        if (pos != registry_.end()) current = pos->second.lock();
        if (current) return current;
        current = std::make_shared<Image>(Image{key, pixels});
        registry_[key] = current;
        return current;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& entry : registry_)
            if (!entry.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { registry_.clear(); }

    std::shared_ptr<Image> try_get(const std::string& key) const {
        auto pos = registry_.find(key);
        if (pos == registry_.end()) return {};
        return pos->second.lock();
    }
};

int main() {
    ImageCache cache;
    std::string action;
    while (std::cin >> action) {
        if (action == "END") break;
        if (action == "LOAD") {
            std::string name;
            int count;
            std::cin >> name >> count;
            {
                std::shared_ptr<Image> img = cache.load(name, count);
                std::cout << "loaded " << img->name << " pixels=" << img->pixel_count << '\n';
            }
        } else if (action == "QUERY") {
            std::string name;
            std::cin >> name;
            if (auto img = cache.try_get(name))
                std::cout << "pixels=" << img->pixel_count << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (action == "DROP_ALL") {
            cache.drop_all();
        }
    }
    return 0;
}
