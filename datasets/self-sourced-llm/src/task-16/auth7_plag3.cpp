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
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto sp = lookup(key)) return sp;
        auto img = std::shared_ptr<Image>(new Image{key, pixels});
        pool_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (const auto& kv : pool_)
            if (!kv.second.expired()) ++n;
        return n;
    }

    void drop_all() { pool_.clear(); }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> pool_;

    std::shared_ptr<Image> lookup(const std::string& key) const {
        auto it = pool_.find(key);
        if (it == pool_.end()) return {};
        return it->second.lock();
    }

public:
    bool query(const std::string& key, int& pixels) const {
        std::shared_ptr<Image> img = lookup(key);
        if (!img) return false;
        pixels = img->pixel_count;
        return true;
    }
};

int main() {
    ImageCache c;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "LOAD") {
            std::string val;
            int v;
            std::cin >> val >> v;
            {
                auto im = c.load(val, v);
                std::cout << "loaded " << im->name << " pixels=" << im->pixel_count << '\n';
            }
        } else if (ln == "QUERY") {
            std::string val;
            std::cin >> val;
            int tmp;
            if (c.query(val, tmp)) std::cout << "pixels=" << tmp << '\n';
            else std::cout << "EXPIRED\n";
        } else if (ln == "DROP_ALL") {
            c.drop_all();
        }
    }
    return 0;
}
