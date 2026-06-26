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
    mutable std::unordered_map<std::string, std::weak_ptr<Image>> store_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = store_.find(key);
        if (it != store_.end()) {
            if (auto sp = it->second.lock()) return sp;
            if (it->second.expired()) store_.erase(it);
        }
        if (auto sp = store_[key].lock()) return sp;
        auto created = std::shared_ptr<Image>(new Image{key, pixels});
        store_[key] = created;
        return created;
    }

    std::size_t size() const {
        std::size_t c = 0;
        for (auto& e : store_)
            if (!e.second.expired()) ++c;
        return c;
    }

    void drop_all() { store_.clear(); }

    bool alive(const std::string& key, int& pixels_out) const {
        auto it = store_.find(key);
        if (it == store_.end()) return false;
        if (auto sp = it->second.lock()) {
            pixels_out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main()
{
    ImageCache ic;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "LOAD") {
            std::string k;
            int p;
            std::cin >> k >> p;
            {
                std::shared_ptr<Image> ptr = ic.load(k, p);
                std::cout << "loaded " << ptr->name << " pixels=" << ptr->pixel_count << '\n';
            }
        } else if (op == "QUERY") {
            std::string k;
            std::cin >> k;
            int p;
            if (ic.alive(k, p)) std::cout << "pixels=" << p << '\n';
            else std::cout << "EXPIRED\n";
        } else if (op == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
