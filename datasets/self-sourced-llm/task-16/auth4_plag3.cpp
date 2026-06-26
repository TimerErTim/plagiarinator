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
        std::weak_ptr<Image>& slot = pool_[key];
        if (std::shared_ptr<Image> hit = slot.lock()) return hit;
        std::shared_ptr<Image> img = std::make_shared<Image>();
        img->name = key;
        img->pixel_count = pixels;
        slot = img;
        return img;
    }

    std::size_t size() const {
        std::size_t count = 0;
        for (const auto& pair : pool_)
            if (!pair.second.expired()) ++count;
        return count;
    }

    void drop_all() { pool_.clear(); }

    std::string query(const std::string& key) const {
        auto found = pool_.find(key);
        if (found == pool_.end()) return "EXPIRED";
        if (std::shared_ptr<Image> live = found->second.lock())
            return "pixels=" + std::to_string(live->pixel_count);
        return "EXPIRED";
    }
};

int main() {
    ImageCache box;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "LOAD") {
            std::string val;
            int n;
            std::cin >> val >> n;
            auto obj = box.load(val, n);
            std::cout << "loaded " << obj->name << " pixels=" << obj->pixel_count << '\n';
            obj.reset();
        } else if (ln == "QUERY") {
            std::string val;
            std::cin >> val;
            std::cout << box.query(val) << '\n';
        } else if (ln == "DROP_ALL") {
            box.drop_all();
        }
    }
    return 0;
}
