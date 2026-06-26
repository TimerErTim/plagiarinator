#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    mutable std::unordered_map<std::string, std::weak_ptr<Image>> map_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            if (auto sp = it->second.lock()) return sp;
            if (it->second.expired()) map_.erase(it);
        }
        if (auto sp = map_[key].lock()) return sp;
        auto created = std::shared_ptr<Image>(new Image{key, pixels});
        map_[key] = created;
        return created;
    }

    std::size_t size() const {
        std::size_t c = 0;
        for (auto& e : map_)
            if (!e.second.expired()) ++c;
        return c;
    }

    void drop_all() { map_.clear(); }

    bool alive(const std::string& key, int& pixels_out) const {
        auto it = map_.find(key);
        if (it == map_.end()) return false;
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
                std::cout << "loaded " << img->name << " pixels=" << img->pixel_count << '\n';
            }
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            int px;
            if (ic.alive(id, px)) std::cout << "pixels=" << px << '\n';
            else std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
