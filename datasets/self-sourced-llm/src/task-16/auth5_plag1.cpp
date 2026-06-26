#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
// cleaned up version

namespace assets {

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto iter = registry_.find(key);
        if (iter != registry_.end()) {
            if (auto strong = iter->second.lock())
                return strong;
        }
        auto image = std::make_shared<Image>(Image{key, pixels});
        registry_[key] = image;
        return image;
    }

    std::size_t size() const {
        std::size_t live = 0;
        for (const auto& item : registry_)
            if (!item.second.expired()) ++live;
        return live;
    }

    void drop_all() { registry_.clear(); }

    bool fetch(const std::string& key, int& pixels) const {
        auto iter = registry_.find(key);
        if (iter == registry_.end()) return false;
        std::shared_ptr<Image> img = iter->second.lock();
        if (!img) return false;
        pixels = img->pixel_count;
        return true;
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> registry_;
};

}  // namespace assets

int main() {
    using assets::ImageCache;
    ImageCache ic;
    std::string token;

    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "LOAD") {
            std::string id;
            int n;
            std::cin >> id >> n;
            {
                auto handle = ic.load(id, n);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << std::endl;
            }
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            int n = 0;
            if (ic.fetch(id, n)) std::cout << "pixels=" << n << std::endl;
            else std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
