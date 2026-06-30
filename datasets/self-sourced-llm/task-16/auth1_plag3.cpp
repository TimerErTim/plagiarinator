#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
// found this online and tweaked it

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> weak_slots_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto cached = weak_slots_[key].lock())
            return cached;
        auto fresh = std::make_shared<Image>(Image{key, pixels});
        weak_slots_[key] = fresh;
        return fresh;
    }

    std::size_t size() const {
        std::size_t cnt = 0;
        for (const auto& pair : weak_slots_)
            if (!pair.second.expired()) ++cnt;
        return cnt;
    }

    void drop_all() { weak_slots_.clear(); }

    bool query_pixels(const std::string& key, int& out) const {
        auto pos = weak_slots_.find(key);
        if (pos == weak_slots_.end()) return false;
        if (auto alive = pos->second.lock()) {
            out = alive->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache img_store;
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "LOAD") {
            std::string id;
            int px_count;
            std::cin >> id >> px_count;
            {
                auto handle = img_store.load(id, px_count);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << '\n';
            }
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            int px_count = 0;
            if (img_store.query_pixels(id, px_count))
                std::cout << "pixels=" << px_count << std::endl;
            else
                std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            img_store.drop_all();
        }
    }
    return 0;
}
