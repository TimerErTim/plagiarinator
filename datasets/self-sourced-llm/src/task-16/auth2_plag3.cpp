#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
// TODO: refactor later

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
        auto it = pool_.find(key);
        if (it != pool_.end()) {
            if (auto locked = it->second.lock())
                return locked;
        }
        auto fresh = std::make_shared<Image>(key, pixels);
        pool_[key] = fresh;
        return fresh;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (auto& p : pool_)
            if (!p.second.expired()) ++n;
        return n;
    }

    void clear() { pool_.clear(); }

    std::shared_ptr<Image> peek(const std::string& key) const {
        auto it = pool_.find(key);
        if (it == pool_.end()) return {};
        return it->second.lock();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> pool_;
};

int main() {
    ImageCache box;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "LOAD") {
            std::string val;
            int p;
            std::cin >> val >> p;
            auto obj = box.load(val, p);
            std::cout << "loaded " << obj->name() << " pixels=" << obj->pixels() << '\n';
            obj.reset();
        } else if (ln == "QUERY") {
            std::string val;
            std::cin >> val;
            if (auto obj = box.peek(val))
                std::cout << "pixels=" << obj->pixels() << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (ln == "DROP_ALL") {
            box.clear();
        }
    }
    return 0;
}
