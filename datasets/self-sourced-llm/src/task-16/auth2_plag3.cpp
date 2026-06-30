#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <utility>
// copied from stackoverflow answer

class Image {
public:
    Image(std::string n, int p) : tag_(std::move(n)), count_(p) {}
    const std::string& name() const { return tag_; }
    int pixels() const { return count_; }

private:
    std::string tag_;
    int count_;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto pos = registry_.find(key);
        if (pos != registry_.end()) {
            if (auto ptr = pos->second.lock())
                return ptr;
        }
        auto made = std::make_shared<Image>(key, pixels);
        registry_[key] = made;
        return made;
    }

    std::size_t size() const {
        std::size_t tally = 0;
        for (auto& entry : registry_)
            if (!entry.second.expired()) ++tally;
        return tally;
    }

    void clear() { registry_.clear(); }

    std::shared_ptr<Image> peek(const std::string& key) const {
        auto pos = registry_.find(key);
        if (pos == registry_.end()) return {};
        return pos->second.lock();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> registry_;
};

int main() {
    ImageCache pool;
    std::string action;
    while (std::cin >> action) {
        if (action == "END") break;
        if (action == "LOAD") {
            std::string id;
            int npx;
            std::cin >> id >> npx;
            auto pic = pool.load(id, npx);
            std::cout << "loaded " << pic->name() << " pixels=" << pic->pixels() << std::endl;
            pic.reset();
        } else if (action == "QUERY") {
            std::string id;
            std::cin >> id;
            if (auto pic = pool.peek(id))
                std::cout << "pixels=" << pic->pixels() << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (action == "DROP_ALL") {
            pool.clear();
        }
    }
    return 0;
}
