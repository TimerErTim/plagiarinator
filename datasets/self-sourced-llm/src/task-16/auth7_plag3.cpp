#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
// reused from last semester

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> weak_map_;

    std::shared_ptr<Image> grab(const std::string& key) const {
        auto pos = weak_map_.find(key);
        if (pos == weak_map_.end()) return {};
        return pos->second.lock();
    }

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto hit = grab(key)) return hit;
        auto pic = std::shared_ptr<Image>(new Image{key, pixels});
        weak_map_[key] = pic;
        return pic;
    }

    std::size_t size() const {
        std::size_t cnt = 0;
        for (const auto& pair : weak_map_)
            if (!pair.second.expired()) ++cnt;
        return cnt;
    }

    void drop_all() { weak_map_.clear(); }

    bool query(const std::string& key, int& pixels) const {
        std::shared_ptr<Image> pic = grab(key);
        if (!pic) return false;
        pixels = pic->pixel_count;
        return true;
    }
};

int main() {
    ImageCache depot;
    std::string cmd_str;
    while (std::cin >> cmd_str) {
        if (cmd_str == "END") break;
        if (cmd_str == "LOAD") {
            std::string id_key;
            int px_amt;
            std::cin >> id_key >> px_amt;
            {
                auto ref = depot.load(id_key, px_amt);
                std::cout << "loaded " << ref->name << " pixels=" << ref->pixel_count << std::endl;
            }
        } else if (cmd_str == "QUERY") {
            std::string id_key;
            std::cin >> id_key;
            int px_amt;
            if (depot.query(id_key, px_amt)) std::cout << "pixels=" << px_amt << '\n';
            else std::cout << "EXPIRED\n";
        } else if (cmd_str == "DROP_ALL") {
            depot.drop_all();
        }
    }
    return 0;
}
