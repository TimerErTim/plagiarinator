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
        std::weak_ptr<Image>& slot = data_[key];
        if (std::shared_ptr<Image> hit = slot.lock()) return hit;
        std::shared_ptr<Image> img = std::make_shared<Image>();
        img->name = key;
        img->pixel_count = pixels;
        slot = img;
        return img;
    }

    std::size_t size() const {
        std::size_t count = 0;
        for (const auto& pair : data_)
            if (!pair.second.expired()) ++count;
        return count;
    }

    void drop_all() { data_.clear(); }

    std::string query(const std::string& key) const {
        auto found = data_.find(key);
        if (found == data_.end()) return "EXPIRED";
        if (std::shared_ptr<Image> live = found->second.lock())
            return "pixels=" + std::to_string(live->pixel_count);
        return "EXPIRED";
    }
};

int main() {
    ImageCache ref;
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "LOAD") {
            std::string tag;
            int n;
            std::cin >> tag >> n;
            auto msg = ref.load(tag, n);
            std::cout << "loaded " << msg->name << " pixels=" << msg->pixel_count << '\n';
            msg.reset();
        } else if (word == "QUERY") {
            std::string tag;
            std::cin >> tag;
            std::cout << ref.query(tag) << '\n';
        } else if (word == "DROP_ALL") {
            ref.drop_all();
        }
    }
    return 0;
}
