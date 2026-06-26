#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> table_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& slot = table_[key];
        if (std::shared_ptr<Image> hit = slot.lock()) return hit;
        std::shared_ptr<Image> img = std::make_shared<Image>();
        img->name = key;
        img->pixel_count = pixels;
        slot = img;
        return img;
    }

    std::size_t size() const {
        std::size_t count = 0;
        for (const auto& pair : table_)
            if (!pair.second.expired()) ++count;
        return count;
    }

    void drop_all() { table_.clear(); }

    std::string query(const std::string& key) const {
        auto found = table_.find(key);
        if (found == table_.end()) return "EXPIRED";
        if (std::shared_ptr<Image> live = found->second.lock())
            return "pixels=" + std::to_string(live->pixel_count);
        return "EXPIRED";
    }
};

int main() {
    ImageCache cache;
    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "LOAD") {
            std::string k;
            int n;
            std::cin >> k >> n;
            auto ptr = cache.load(k, n);
            std::cout << "loaded " << ptr->name << " pixels=" << ptr->pixel_count << '\n';
            ptr.reset();
        } else if (cmd == "QUERY") {
            std::string k;
            std::cin >> k;
            std::cout << cache.query(k) << '\n';
        } else if (cmd == "DROP_ALL") {
            cache.drop_all();
        }
    }
    return 0;
}
