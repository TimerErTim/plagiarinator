#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
// modified from tutorial code

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> store_map_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::shared_ptr<Image> existing;
        auto idx = store_map_.find(key);
        if (idx != store_map_.end()) existing = idx->second.lock();
        if (existing) return existing;
        existing = std::make_shared<Image>(Image{key, pixels});
        store_map_[key] = existing;
        return existing;
    }

    std::size_t size() const {
        std::size_t num = 0;
        for (const auto& rec : store_map_)
            if (!rec.second.expired()) ++num;
        return num;
    }

    void drop_all() { store_map_.clear(); }

    std::shared_ptr<Image> try_get(const std::string& key) const {
        auto idx = store_map_.find(key);
        if (idx == store_map_.end()) return {};
        return idx->second.lock();
    }
};

int main() {
    ImageCache img_cache;
    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "LOAD") {
            std::string fname;
            int px_cnt;
            std::cin >> fname >> px_cnt;
            {
                std::shared_ptr<Image> ptr = img_cache.load(fname, px_cnt);
                std::cout << "loaded " << ptr->name << " pixels=" << ptr->pixel_count << std::endl;
            }
        } else if (verb == "QUERY") {
            std::string fname;
            std::cin >> fname;
            if (auto ptr = img_cache.try_get(fname))
                std::cout << "pixels=" << ptr->pixel_count << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (verb == "DROP_ALL") {
            img_cache.drop_all();
        }
    }
    return 0;
}
