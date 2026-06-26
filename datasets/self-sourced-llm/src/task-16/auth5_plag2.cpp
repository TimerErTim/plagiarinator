#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
// adapted from classmate submission

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

int main()
{
    using assets::ImageCache;
    ImageCache store;
    std::string op;

    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "LOAD") {
            std::string k;
            int p;
            std::cin >> k >> p;
            {
                auto handle = store.load(k, p);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << '\n';
            }
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            int p = 0;
            if (store.fetch(k, p)) std::cout << "pixels=" << p << '\n';
            else std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            store.drop_all();
        }
    }
    return 0;
}
