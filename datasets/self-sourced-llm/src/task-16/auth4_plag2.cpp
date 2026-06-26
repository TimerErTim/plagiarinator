#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
// adapted from classmate submission

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> registry_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& slot = registry_[key];
        if (std::shared_ptr<Image> hit = slot.lock()) return hit;
        std::shared_ptr<Image> img = std::make_shared<Image>();
        img->name = key;
        img->pixel_count = pixels;
        slot = img;
        return img;
    }

    std::size_t size() const {
        std::size_t count = 0;
        for (const auto& pair : registry_)
            if (!pair.second.expired()) ++count;
        return count;
    }

    void drop_all() { registry_.clear(); }

    std::string query(const std::string& key) const {
        auto found = registry_.find(key);
        if (found == registry_.end()) return "EXPIRED";
        if (std::shared_ptr<Image> live = found->second.lock())
            return "pixels=" + std::to_string(live->pixel_count);
        return "EXPIRED";
    }
};

int main()
{
    ImageCache store;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "LOAD") {
            std::string k;
            int n;
            std::cin >> k >> n;
            auto ptr = store.load(k, n);
            std::cout << "loaded " << ptr->name << " pixels=" << ptr->pixel_count << '\n';
            ptr.reset();
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            std::cout << store.query(k) << '\n';
        } else if (op == "DROP_ALL") {
            store.drop_all();
        }
    }
    return 0;
}
