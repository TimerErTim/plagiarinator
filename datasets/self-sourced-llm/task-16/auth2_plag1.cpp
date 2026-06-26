#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
// cleaned up version

class Image {
public:
    Image(std::string n, int p) : label_(std::move(n)), count_(p) {}
    const std::string& name() const { return label_; }
    int pixels() const { return count_; }

private:
    std::string label_;
    int count_;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = registry_.find(key);
        if (it != registry_.end()) {
            if (auto locked = it->second.lock())
                return locked;
        }
        auto fresh = std::make_shared<Image>(key, pixels);
        registry_[key] = fresh;
        return fresh;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (auto& p : registry_)
            if (!p.second.expired()) ++n;
        return n;
    }

    void clear() { registry_.clear(); }

    std::shared_ptr<Image> peek(const std::string& key) const {
        auto it = registry_.find(key);
        if (it == registry_.end()) return {};
        return it->second.lock();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> registry_;
};

int main() {
    ImageCache ic;
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "LOAD") {
            std::string id;
            int p;
            std::cin >> id >> p;
            auto handle = ic.load(id, p);
            std::cout << "loaded " << handle->name() << " pixels=" << handle->pixels() << std::endl;
            handle.reset();
        } else if (token == "QUERY") {
            std::string id;
            std::cin >> id;
            if (auto handle = ic.peek(id))
                std::cout << "pixels=" << handle->pixels() << std::endl;
            else
                std::cout << "EXPIRED\n";
        } else if (token == "DROP_ALL") {
            ic.clear();
        }
    }
    return 0;
}
