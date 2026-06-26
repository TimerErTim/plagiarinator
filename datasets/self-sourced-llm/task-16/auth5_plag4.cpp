#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

namespace assets {

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto iter = data_.find(key);
        if (iter != data_.end()) {
            if (auto strong = iter->second.lock())
                return strong;
        }
        auto image = std::make_shared<Image>(Image{key, pixels});
        data_[key] = image;
        return image;
    }

    std::size_t size() const {
        std::size_t live = 0;
        for (const auto& item : data_)
            if (!item.second.expired()) ++live;
        return live;
    }

    void drop_all() { data_.clear(); }

    bool fetch(const std::string& key, int& pixels) const {
        auto iter = data_.find(key);
        if (iter == data_.end()) return false;
        std::shared_ptr<Image> img = iter->second.lock();
        if (!img) return false;
        pixels = img->pixel_count;
        return true;
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> data_;
};

}  // namespace assets

int main() {
    using assets::ImageCache;
    ImageCache ref;
    std::string elem;

    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem == "LOAD") {
            std::string tag;
            int num;
            std::cin >> tag >> num;
            {
                auto handle = ref.load(tag, num);
                std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << '\n';
            }
        } else if (elem == "QUERY") {
            std::string tag;
            std::cin >> tag;
            int num = 0;
            if (ref.fetch(tag, num)) std::cout << "pixels=" << num << '\n';
            else std::cout << "EXPIRED\n";
        } else if (elem == "DROP_ALL") {
            ref.drop_all();
        }
    }
    return 0;
}
