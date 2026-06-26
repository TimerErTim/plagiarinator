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
    std::unordered_map<std::string, std::weak_ptr<Image>> store_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto existing = store_[key].lock())
            return existing;
        auto img = std::make_shared<Image>(Image{key, pixels});
        store_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& kv : store_)
            if (!kv.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { store_.clear(); }

    bool query_pixels(const std::string& key, int& out) const {
        auto it = store_.find(key);
        if (it == store_.end()) return false;
        if (auto sp = it->second.lock()) {
            out = sp->pixel_count;
            return true;
        }
        return false;
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
            int p;
            std::cin >> k >> p;
            {
                auto ptr = store.load(k, p);
                std::cout << "loaded " << ptr->name << " pixels=" << ptr->pixel_count << '\n';
            }
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            int p = 0;
            if (store.query_pixels(k, p))
                std::cout << "pixels=" << p << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            store.drop_all();
        }
    }
    return 0;
}
