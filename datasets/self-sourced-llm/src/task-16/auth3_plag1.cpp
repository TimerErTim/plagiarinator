#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    mutable std::unordered_map<std::string, std::weak_ptr<Image>> cache_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            if (auto sp = it->second.lock()) return sp;
            if (it->second.expired()) cache_.erase(it);
        }
        if (auto sp = cache_[key].lock()) return sp;
        auto created = std::shared_ptr<Image>(new Image{key, pixels});
        cache_[key] = created;
        return created;
    }

    std::size_t size() const {
        std::size_t c = 0;
        for (auto& e : cache_)
            if (!e.second.expired()) c++;
        return c;
    }

    void drop_all() { cache_.clear(); }

    bool alive(const std::string& key, int& pixels_out) const {
        auto it = cache_.find(key);
        if (it == cache_.end()) return false;
        if (auto sp = it->second.lock()) {
            pixels_out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache ic;
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "LOAD") {
            std::string id;
            int px;
            std::cin >> id >> px;
            {
                std::shared_ptr<Image> img = ic.load(id, px);
                std::cout << "loaded " << img->name << " pixels=" << img->pixel_count << std::endl;
            }
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            int px;
            if (ic.alive(id, px)) std::cout << "pixels=" << px << std::endl;
            else std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
