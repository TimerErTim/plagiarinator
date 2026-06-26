#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    mutable std::unordered_map<std::string, std::weak_ptr<Image>> data_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = data_.find(key);
        if (it != data_.end()) {
            if (auto sp = it->second.lock()) return sp;
            if (it->second.expired()) data_.erase(it);
        }
        if (auto sp = data_[key].lock()) return sp;
        auto created = std::shared_ptr<Image>(new Image{key, pixels});
        data_[key] = created;
        return created;
    }

    std::size_t size() const {
        std::size_t c = 0;
        for (auto& e : data_)
            if (!e.second.expired()) ++c;
        return c;
    }

    void drop_all() { data_.clear(); }

    bool alive(const std::string& key, int& pixels_out) const {
        auto it = data_.find(key);
        if (it == data_.end()) return false;
        if (auto sp = it->second.lock()) {
            pixels_out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache ic;
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "LOAD") {
            std::string id;
            int num;
            std::cin >> id >> num;
            {
                std::shared_ptr<Image> msg = ic.load(id, num);
                std::cout << "loaded " << msg->name << " pixels=" << msg->pixel_count << '\n';
            }
        } else if (elem == "QUERY") {
            std::string id;
            std::cin >> id;
            int num;
            if (ic.alive(id, num)) std::cout << "pixels=" << num << '\n';
            else std::cout << "EXPIRED\n";
        } else if (elem == "DROP_ALL") {
            ic.drop_all();
        }
    }
    return 0;
}
