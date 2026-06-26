#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

namespace assets {

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto iter = cache_.find(key);
        if (iter != cache_.end()) {
            if (auto strong = iter->second.lock())
                return strong;
        }
        auto image = std::make_shared<Image>(Image{key, pixels});
        cache_[key] = image;
        return image;
    }

    std::size_t size() const {
        std::size_t live = 0;
        for (const auto& item : cache_)
            if (!item.second.expired()) ++live;
        return live;
    }

    void drop_all() { cache_.clear(); }

    bool fetch(const std::string& key, int& pixels) const {
        auto iter = cache_.find(key);
        if (iter == cache_.end()) return false;
        std::shared_ptr<Image> img = iter->second.lock();
        if (!img) return false;
        pixels = img->pixel_count;
        return true;
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> cache_;
};

}  // namespace assets

int main() {
    using assets::ImageCache;
    ImageCache cache;
    std::string verb;

    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "LOAD") {
            std::string key;
            int px;
            std::cin >> key >> px;
            {
                auto handle = cache.load(key, px);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << '\n';
            }
        } else if (verb == "QUERY") {
            std::string key;
            std::cin >> key;
            int px = 0;
            if (cache.fetch(key, px)) std::cout << "pixels=" << px << '\n';
            else std::cout << "EXPIRED\n";
        } else if (verb == "DROP_ALL") {
            cache.drop_all();
        }
    }
    return 0;
}
