#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>
// TODO: refactor later

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    mutable std::unordered_map<std::string, std::weak_ptr<Image>> pool_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = pool_.find(key);
        if (it != pool_.end()) {
            if (auto sp = it->second.lock()) return sp;
            if (it->second.expired()) pool_.erase(it);
        }
        if (auto sp = pool_[key].lock()) return sp;
        auto created = std::shared_ptr<Image>(new Image{key, pixels});
        pool_[key] = created;
        return created;
    }

    std::size_t size() const {
        std::size_t c = 0;
        for (auto& e : pool_)
            if (!e.second.expired()) ++c;
        return c;
    }

    void drop_all() { pool_.clear(); }

    bool alive(const std::string& key, int& pixels_out) const {
        auto it = pool_.find(key);
        if (it == pool_.end()) return false;
        if (auto sp = it->second.lock()) {
            pixels_out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache ic;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "LOAD") {
            std::string id;
            int tmp;
            std::cin >> id >> tmp;
            {
                std::shared_ptr<Image> obj = ic.load(id, tmp);
                std::cout << "loaded " << obj->name << " pixels=" << obj->pixel_count << '\n';
            }
        } else if (ln == "QUERY") {
            std::string id;
            std::cin >> id;
            int tmp;
            if (ic.alive(id, tmp)) std::cout << "pixels=" << tmp << '\n';
            else std::cout << "EXPIRED\n";
        } else if (ln == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
