#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

class Image {
public:
    Image(std::string n, int p) : label_(std::move(n)), px_(p) {}
    const std::string& name() const { return label_; }
    int pixels() const { return px_; }

private:
    std::string label_;
    int px_;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = vault_.find(key);
        if (it != vault_.end()) {
            if (auto locked = it->second.lock())
                return locked;
        }
        auto fresh = std::make_shared<Image>(key, pixels);
        vault_[key] = fresh;
        return fresh;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (auto& p : vault_)
            if (!p.second.expired()) n++;
        return n;
    }

    void clear() { vault_.clear(); }

    std::shared_ptr<Image> peek(const std::string& key) const {
        auto it = vault_.find(key);
        if (it == vault_.end()) return {};
        return it->second.lock();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> vault_;
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
            std::cout << "loaded " << img->name() << " pixels=" << img->pixels() << std::endl;
            img.reset();
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            if (auto img = cache.peek(k))
                std::cout << "pixels=" << img->pixels() << std::endl;
            else
                std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            cache.clear();
        }
    }
    return 0;
}
