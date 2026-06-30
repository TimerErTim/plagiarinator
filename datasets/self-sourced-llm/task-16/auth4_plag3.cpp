#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>
// quick hack for assignment

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> lookup_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& ref = lookup_[key];
        if (std::shared_ptr<Image> got = ref.lock()) return got;
        std::shared_ptr<Image> pic = std::make_shared<Image>();
        pic->name = key;
        pic->pixel_count = pixels;
        ref = pic;
        return pic;
    }

    std::size_t size() const {
        std::size_t n_live = 0;
        for (const auto& item : lookup_)
            if (!item.second.expired()) ++n_live;
        return n_live;
    }

    void drop_all() { lookup_.clear(); }

    std::string query(const std::string& key) const {
        auto loc = lookup_.find(key);
        if (loc == lookup_.end()) return "EXPIRED";
        if (std::shared_ptr<Image> active = loc->second.lock())
            return "pixels=" + std::to_string(active->pixel_count);
        return "EXPIRED";
    }
};

int main() {
    ImageCache box;
    std::string opcode;
    while (std::cin >> opcode) {
        if (opcode == "END") break;
        if (opcode == "LOAD") {
            std::string label;
            int px_amt;
            std::cin >> label >> px_amt;
            auto handle = box.load(label, px_amt);
            std::cout << "loaded " << handle->name << " pixels=" << handle->pixel_count << std::endl;
            handle.reset();
        } else if (opcode == "QUERY") {
            std::string label;
            std::cin >> label;
            std::cout << box.query(label) << '\n';
        } else if (opcode == "DROP_ALL") {
            box.drop_all();
        }
    }
    return 0;
}
