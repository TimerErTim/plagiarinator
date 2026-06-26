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

    std::shared_ptr<Image> create(const std::string& key, int pixels) {
        return std::make_shared<Image>(Image{key, pixels});
    }

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& weak = pool_[key];
        std::shared_ptr<Image> strong = weak.lock();
        if (strong) return strong;
        strong = create(key, pixels);
        weak = strong;
        return strong;
    }

    std::size_t size() const {
        std::size_t s = 0;
        for (auto const& e : pool_)
            if (!e.second.expired()) ++s;
        return s;
    }

    void drop_all() { pool_.clear(); }

    void print_query(const std::string& key) const {
        auto it = pool_.find(key);
        if (it == pool_.end()) {
            std::cout << "EXPIRED\n";
            return;
        }
        std::shared_ptr<Image> img = it->second.lock();
        if (!img) std::cout << "EXPIRED\n";
        else std::cout << "pixels=" << img->pixel_count << '\n';
    }
};

int main() {
    ImageCache box;
    std::string buf;
    while (std::cin >> buf) {
        if (buf == "END") break;
        if (buf == "LOAD") {
            std::string val;
            int p;
            std::cin >> val >> p;
            std::shared_ptr<Image> tmp = box.load(val, p);
            std::cout << "loaded " << tmp->name << " pixels=" << tmp->pixel_count << '\n';
        } else if (buf == "QUERY") {
            std::string val;
            std::cin >> val;
            box.print_query(val);
        } else if (buf == "DROP_ALL") {
            box.drop_all();
        }
    }
    return 0;
}
