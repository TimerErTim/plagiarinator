#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
// shared sol from discord

namespace assets {

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto spot = cache_.find(key);
        if (spot != cache_.end()) {
            if (auto kept = spot->second.lock())
                return kept;
        }
        auto fresh_img = std::make_shared<Image>(Image{key, pixels});
        cache_[key] = fresh_img;
        return fresh_img;
    }

    std::size_t size() const {
        std::size_t active = 0;
        for (const auto& node : cache_)
            if (!node.second.expired()) ++active;
        return active;
    }

    void drop_all() { cache_.clear(); }

    bool fetch(const std::string& key, int& pixels) const {
        auto spot = cache_.find(key);
        if (spot == cache_.end()) return false;
        std::shared_ptr<Image> obj = spot->second.lock();
        if (!obj) return false;
        pixels = obj->pixel_count;
        return true;
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> cache_;
};

}  // namespace assets

int main() {
    using assets::ImageCache;
    ImageCache store;
    std::string action;

    while (std::cin >> action) {
        if (action == "END") break;
        if (action == "LOAD") {
            std::string tag;
            int px_num;
            std::cin >> tag >> px_num;
            {
                auto ref = store.load(tag, px_num);
                std::cout << "loaded " << ref->name << " pixels=" << ref->pixel_count << std::endl;
            }
        } else if (action == "QUERY") {
            std::string tag;
            std::cin >> tag;
            int px_num = 0;
            if (store.fetch(tag, px_num)) std::cout << "pixels=" << px_num << '\n';
            else std::cout << "EXPIRED\n";
        } else if (action == "DROP_ALL") {
            store.drop_all();
        }
    }
    return 0;
}
