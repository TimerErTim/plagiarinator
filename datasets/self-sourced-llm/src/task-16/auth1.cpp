#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> entries_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto existing = entries_[key].lock())
            return existing;
        auto img = std::make_shared<Image>(Image{key, pixels});
        entries_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& kv : entries_)
            if (!kv.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { entries_.clear(); }

    bool query_pixels(const std::string& key, int& out) const {
        auto it = entries_.find(key);
        if (it == entries_.end()) return false;
        if (auto sp = it->second.lock()) {
            out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache cache;
    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "LOAD") {
            std::string key;
            int px;
            std::cin >> key >> px;
            {
                auto img = cache.load(key, px);
                std::cout << "loaded " << img->name << " pixels=" << img->pixel_count << '\n';
            }
        } else if (cmd == "QUERY") {
            std::string key;
            std::cin >> key;
            int px = 0;
            if (cache.query_pixels(key, px))
                std::cout << "pixels=" << px << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (cmd == "DROP_ALL") {
            cache.drop_all();
        }
    }
    return 0;
}
