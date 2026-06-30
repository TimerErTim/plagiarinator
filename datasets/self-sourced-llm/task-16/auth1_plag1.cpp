#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> bucket_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto hit = bucket_[key].lock())
            return hit;
        auto img = std::make_shared<Image>(Image{key, pixels});
        bucket_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t live = 0;
        for (const auto& kv : bucket_)
            if (!kv.second.expired()) live++;
        return live;
    }

    void drop_all() { bucket_.clear(); }

    bool query_pixels(const std::string& key, int& out) const {
        auto it = bucket_.find(key);
        if (it == bucket_.end()) return false;
        if (auto sp = it->second.lock()) {
            out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache cache;
    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "LOAD") {
            std::string key;
            int px;
            std::cin >> key >> px;
            {
                auto img = cache.load(key, px);
                std::cout << "loaded " << img->name << " pixels=" << img->pixel_count << std::endl;
            }
        } else if (verb == "QUERY") {
            std::string key;
            std::cin >> key;
            int px = 0;
            if (cache.query_pixels(key, px))
                std::cout << "pixels=" << px << std::endl;
            else
                std::cout << "EXPIRED\n";
        } else if (verb == "DROP_ALL") {
            cache.drop_all();
        }
    }
    return 0;
}
