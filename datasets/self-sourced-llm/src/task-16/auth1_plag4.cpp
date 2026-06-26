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
        if (auto existing = data_[key].lock())
            return existing;
        auto img = std::make_shared<Image>(Image{key, pixels});
        data_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t alive = 0;
        for (const auto& kv : data_)
            if (!kv.second.expired()) ++alive;
        return alive;
    }

    void drop_all() { data_.clear(); }

    bool query_pixels(const std::string& key, int& out) const {
        auto it = data_.find(key);
        if (it == data_.end()) return false;
        if (auto sp = it->second.lock()) {
            out = sp->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache ref;
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "LOAD") {
            std::string tag;
            int num;
            std::cin >> tag >> num;
            {
                auto msg = ref.load(tag, num);
                std::cout << "loaded " << msg->name << " pixels=" << msg->pixel_count << '\n';
            }
        } else if (word == "QUERY") {
            std::string tag;
            std::cin >> tag;
            int num = 0;
            if (ref.query_pixels(tag, num))
                std::cout << "pixels=" << num << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (word == "DROP_ALL") {
            ref.drop_all();
        }
    }
    return 0;
}
