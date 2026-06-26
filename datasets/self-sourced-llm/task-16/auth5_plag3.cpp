#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
// TODO: refactor later

namespace assets {

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto iter = pool_.find(key);
        if (iter != pool_.end()) {
            if (auto strong = iter->second.lock())
                return strong;
        }
        auto image = std::make_shared<Image>(Image{key, pixels});
        pool_[key] = image;
        return image;
    }

    std::size_t size() const {
        std::size_t live = 0;
        for (const auto& item : pool_)
            if (!item.second.expired()) ++live;
        return live;
    }

    void drop_all() { pool_.clear(); }

    bool fetch(const std::string& key, int& pixels) const {
        auto iter = pool_.find(key);
        if (iter == pool_.end()) return false;
        std::shared_ptr<Image> img = iter->second.lock();
        if (!img) return false;
        pixels = img->pixel_count;
        return true;
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> pool_;
};

}  // namespace assets

int main() {
    using assets::ImageCache;
    ImageCache box;
    std::string ln;

    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "LOAD") {
            std::string val;
            int tmp;
            std::cin >> val >> tmp;
            {
                auto handle = box.load(val, tmp);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << '\n';
            }
        } else if (ln == "QUERY") {
            std::string val;
            std::cin >> val;
            int tmp = 0;
            if (box.fetch(val, tmp)) std::cout << "pixels=" << tmp << '\n';
            else std::cout << "EXPIRED\n";
        } else if (ln == "DROP_ALL") {
            box.drop_all();
        }
    }
    return 0;
}
