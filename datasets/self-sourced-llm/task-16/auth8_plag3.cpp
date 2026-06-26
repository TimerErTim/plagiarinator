#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
// TODO: refactor later

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> pool_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::shared_ptr<Image> current;
        auto pos = pool_.find(key);
        if (pos != pool_.end()) current = pos->second.lock();
        if (current) return current;
        current = std::make_shared<Image>(Image{key, pixels});
        pool_[key] = current;
        return current;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& entry : pool_)
            if (!entry.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { pool_.clear(); }

    std::shared_ptr<Image> try_get(const std::string& key) const {
        auto pos = pool_.find(key);
        if (pos == pool_.end()) return {};
        return pos->second.lock();
    }
};

int main() {
    ImageCache box;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "LOAD") {
            std::string name;
            int count;
            std::cin >> name >> count;
            {
                std::shared_ptr<Image> obj = box.load(name, count);
                std::cout << "loaded " << obj->name << " pixels=" << obj->pixel_count << '\n';
            }
        } else if (ln == "QUERY") {
            std::string name;
            std::cin >> name;
            if (auto obj = box.try_get(name))
                std::cout << "pixels=" << obj->pixel_count << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (ln == "DROP_ALL") {
            box.drop_all();
        }
    }
    return 0;
}
