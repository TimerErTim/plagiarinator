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
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto sp = lookup(key)) return sp;
        auto img = std::shared_ptr<Image>(new Image{key, pixels});
        content_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (const auto& kv : content_)
            if (!kv.second.expired()) ++n;
        return n;
    }

    void drop_all() { content_.clear(); }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> content_;

    std::shared_ptr<Image> lookup(const std::string& key) const {
        auto it = content_.find(key);
        if (it == content_.end()) return {};
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
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "LOAD") {
            std::string id;
            int v;
            std::cin >> id >> v;
            {
                auto handle = c.load(id, v);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << std::endl;
            }
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            int n;
            if (c.query(id, n)) std::cout << "pixels=" << n << std::endl;
            else std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            c.drop_all();
        }
    }
    return 0;
}
