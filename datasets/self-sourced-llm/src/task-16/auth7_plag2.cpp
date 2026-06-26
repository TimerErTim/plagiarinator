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
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto sp = lookup(key)) return sp;
        auto img = std::shared_ptr<Image>(new Image{key, pixels});
        store_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (const auto& kv : store_)
            if (!kv.second.expired()) ++n;
        return n;
    }

    void drop_all() { store_.clear(); }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> store_;

    std::shared_ptr<Image> lookup(const std::string& key) const {
        auto it = store_.find(key);
        if (it == store_.end()) return {};
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

int main()
{
    ImageCache c;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "LOAD") {
            std::string k;
            int v;
            std::cin >> k >> v;
            {
                auto im = c.load(k, v);
                std::cout << "loaded " << im->name << " pixels=" << im->pixel_count << '\n';
            }
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            int p;
            if (c.query(k, p)) std::cout << "pixels=" << p << '\n';
            else std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            c.drop_all();
        }
    }
    return 0;
}
