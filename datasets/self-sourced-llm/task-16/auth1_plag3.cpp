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
        if (auto existing = pool_[key].lock())
            return existing;
        auto img = std::make_shared<Image>(Image{key, pixels});
        pool_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& kv : pool_)
            if (!kv.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { pool_.clear(); }

    bool query_pixels(const std::string& key, int& out) const {
        auto it = pool_.find(key);
        if (it == pool_.end()) return false;
        if (auto sp = it->second.lock()) {
            out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache box;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "LOAD") {
            std::string val;
            int tmp;
            std::cin >> val >> tmp;
            {
                auto obj = box.load(val, tmp);
                std::cout << "loaded " << obj->name << " pixels=" << obj->pixel_count << '\n';
            }
        } else if (ln == "QUERY") {
            std::string val;
            std::cin >> val;
            int tmp = 0;
            if (box.query_pixels(val, tmp))
                std::cout << "pixels=" << tmp << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (ln == "DROP_ALL") {
            box.drop_all();
        }
    }
    return 0;
}
