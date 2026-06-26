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
    std::unordered_map<std::string, std::weak_ptr<Image>> data_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::shared_ptr<Image> current;
        auto pos = data_.find(key);
        if (pos != data_.end()) current = pos->second.lock();
        if (current) return current;
        current = std::make_shared<Image>(Image{key, pixels});
        data_[key] = current;
        return current;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& entry : data_)
            if (!entry.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { data_.clear(); }

    std::shared_ptr<Image> try_get(const std::string& key) const {
        auto pos = data_.find(key);
        if (pos == data_.end()) return {};
        return pos->second.lock();
    }
};

int main() {
    ImageCache ref;
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "LOAD") {
            std::string name;
            int count;
            std::cin >> name >> count;
            {
                std::shared_ptr<Image> msg = ref.load(name, count);
                std::cout << "loaded " << msg->name << " pixels=" << msg->pixel_count << '\n';
            }
        } else if (elem == "QUERY") {
            std::string name;
            std::cin >> name;
            if (auto msg = ref.try_get(name))
                std::cout << "pixels=" << msg->pixel_count << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (elem == "DROP_ALL") {
            ref.drop_all();
        }
    }
    return 0;
}
