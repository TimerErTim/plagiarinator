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
    std::unordered_map<std::string, std::weak_ptr<Image>> content_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& slot = content_[key];
        if (std::shared_ptr<Image> hit = slot.lock()) return hit;
        std::shared_ptr<Image> img = std::make_shared<Image>();
        img->name = key;
        img->pixel_count = pixels;
        slot = img;
        return img;
    }

    std::size_t size() const {
        std::size_t count = 0;
        for (const auto& pair : content_)
            if (!pair.second.expired()) ++count;
        return count;
    }

    void drop_all() { content_.clear(); }

    std::string query(const std::string& key) const {
        auto found = content_.find(key);
        if (found == content_.end()) return "EXPIRED";
        if (std::shared_ptr<Image> live = found->second.lock())
            return "pixels=" + std::to_string(live->pixel_count);
        return "EXPIRED";
    }
};

int main() {
    ImageCache ic;
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "LOAD") {
            std::string id;
            int n;
            std::cin >> id >> n;
            auto handle = ic.load(id, n);
            std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << std::endl;
            handle.reset();
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            std::cout << ic.query(id) << std::endl;
        } else if (token == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
