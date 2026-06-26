#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

class Image {
public:
    Image(std::string n, int p) : name_(std::move(n)), pixels_(p) {}
    const std::string& name() const { return name_; }
    int pixels() const { return pixels_; }

private:
    std::string name_;
    int pixels_;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = store_.find(key);
        if (it != store_.end()) {
            if (auto locked = it->second.lock())
                return locked;
        }
        auto fresh = std::make_shared<Image>(key, pixels);
        store_[key] = fresh;
        return fresh;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (auto& p : store_)
            if (!p.second.expired()) ++n;
        return n;
    }

    void clear() { store_.clear(); }

    std::shared_ptr<Image> peek(const std::string& key) const {
        auto it = store_.find(key);
        if (it == store_.end()) return {};
        return it->second.lock();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> store_;
};

int main() {
    ImageCache cache;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "LOAD") {
            std::string k;
            int p;
            std::cin >> k >> p;
            auto img = cache.load(k, p);
            std::cout << "loaded " << img->name() << " pixels=" << img->pixels() << '\n';
            img.reset();
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            if (auto img = cache.peek(k))
                std::cout << "pixels=" << img->pixels() << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            cache.clear();
        }
    }
    return 0;
}

#include <utility>
