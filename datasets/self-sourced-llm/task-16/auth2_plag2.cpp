#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
// adapted from classmate submission

class Image {
public:
    Image(std::string n, int p) : title_(std::move(n)), px_(p) {}
    const std::string& name() const { return title_; }
    int pixels() const { return px_; }

private:
    std::string title_;
    int px_;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = entries_.find(key);
        if (it != entries_.end()) {
            if (auto locked = it->second.lock())
                return locked;
        }
        auto fresh = std::make_shared<Image>(key, pixels);
        entries_[key] = fresh;
        return fresh;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (auto& p : entries_)
            if (!p.second.expired()) ++n;
        return n;
    }

    void clear() { entries_.clear(); }

    std::shared_ptr<Image> peek(const std::string& key) const {
        auto it = entries_.find(key);
        if (it == entries_.end()) return {};
        return it->second.lock();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> entries_;
};

int main()
{
    ImageCache store;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "LOAD") {
            std::string k;
            int p;
            std::cin >> k >> p;
            auto ptr = store.load(k, p);
            std::cout << "loaded " << ptr->name() << " pixels=" << ptr->pixels() << '\n';
            ptr.reset();
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            if (auto ptr = store.peek(k))
                std::cout << "pixels=" << ptr->pixels() << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            store.clear();
        }
    }
    return 0;
}
