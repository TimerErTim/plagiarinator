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
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        if (auto sp = lookup(key)) return sp;
        auto img = std::shared_ptr<Image>(new Image{key, pixels});
        data_[key] = img;
        return img;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (const auto& kv : data_)
            if (!kv.second.expired()) ++n;
        return n;
    }

    void drop_all() { data_.clear(); }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> data_;

    std::shared_ptr<Image> lookup(const std::string& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) return {};
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

int main() {
    ImageCache c;
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "LOAD") {
            std::string tag;
            int v;
            std::cin >> tag >> v;
            {
                auto im = c.load(tag, v);
                std::cout << "loaded " << im->name << " pixels=" << im->pixel_count << '\n';
            }
        } else if (word == "QUERY") {
            std::string tag;
            std::cin >> tag;
            int num;
            if (c.query(tag, num)) std::cout << "pixels=" << num << '\n';
            else std::cout << "EXPIRED\n";
        } else if (word == "DROP_ALL") {
            c.drop_all();
        }
    }
    return 0;
}
