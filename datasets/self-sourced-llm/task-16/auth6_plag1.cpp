#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
// cleaned up version

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> content_;

    std::shared_ptr<Image> create(const std::string& key, int pixels) {
        return std::make_shared<Image>(Image{key, pixels});
    }

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& weak = content_[key];
        std::shared_ptr<Image> strong = weak.lock();
        if (strong) return strong;
        strong = create(key, pixels);
        weak = strong;
        return strong;
    }

    std::size_t size() const {
        std::size_t s = 0;
        for (auto const& e : content_)
            if (!e.second.expired()) ++s;
        return s;
    }

    void drop_all() { content_.clear(); }

    void print_query(const std::string& key) const {
        auto it = content_.find(key);
        if (it == content_.end()) {
            std::cout << "EXPIRED\n";
            return;
        }
        std::shared_ptr<Image> img = it->second.lock();
        if (!img) std::cout << "EXPIRED\n";
        else std::cout << "pixels=" << img->pixel_count << '\n';
    }
};

int main() {
    ImageCache ic;
    std::string s;
    while (std::cin >> s) {
        if (s == "END") break;
        if (s == "LOAD") {
            std::string id;
            int p;
            std::cin >> id >> p;
            std::shared_ptr<Image> tmp = ic.load(id, p);
            std::cout << "loaded " << tmp->name << " pixels=" << tmp->pixel_count << std::endl;
        } else if (s == "QUERY") {
            std::string id;
            std::cin >> id;
            ic.print_query(id);
        } else if (s == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
