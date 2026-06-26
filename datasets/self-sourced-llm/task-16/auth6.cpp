#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> data_;

    std::shared_ptr<Image> create(const std::string& key, int pixels) {
        return std::make_shared<Image>(Image{key, pixels});
    }

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& weak = data_[key];
        std::shared_ptr<Image> strong = weak.lock();
        if (strong) return strong;
        strong = create(key, pixels);
        weak = strong;
        return strong;
    }

    std::size_t size() const {
        std::size_t s = 0;
        for (auto const& e : data_)
            if (!e.second.expired()) ++s;
        return s;
    }

    void drop_all() { data_.clear(); }

    void print_query(const std::string& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) {
            std::cout << "EXPIRED\n";
            return;
        }
        std::shared_ptr<Image> img = it->second.lock();
        if (!img) std::cout << "EXPIRED\n";
        else std::cout << "pixels=" << img->pixel_count << '\n';
    }
};

int main() {
    ImageCache cache;
    std::string line;
    while (std::cin >> line) {
        if (line == "END") break;
        if (line == "LOAD") {
            std::string k;
            int p;
            std::cin >> k >> p;
            std::shared_ptr<Image> tmp = cache.load(k, p);
            std::cout << "loaded " << tmp->name << " pixels=" << tmp->pixel_count << '\n';
        } else if (line == "QUERY") {
            std::string k;
            std::cin >> k;
            cache.print_query(k);
        } else if (line == "DROP_ALL") {
            cache.drop_all();
        }
    }
    return 0;
}
