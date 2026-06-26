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
    std::unordered_map<std::string, std::weak_ptr<Image>> registry_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto existing = registry_[key].lock())
            return existing;
        auto img = std::make_shared<Image>(Image{key, pixels});
        registry_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& kv : registry_)
            if (!kv.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { registry_.clear(); }

    bool query_pixels(const std::string& key, int& out) const {
        auto it = registry_.find(key);
        if (it == registry_.end()) return false;
        if (auto sp = it->second.lock()) {
            out = sp->pixel_count;
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
            int n;
            std::cin >> id >> n;
            {
                auto handle = ic.load(id, n);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << std::endl;
            }
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            int n = 0;
            if (ic.query_pixels(id, n))
                std::cout << "pixels=" << n << std::endl;
            else
                std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
