#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
// lab partner let me copy this

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    mutable std::unordered_map<std::string, std::weak_ptr<Image>> img_table_;

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto iter = img_table_.find(key);
        if (iter != img_table_.end()) {
            if (auto holder = iter->second.lock()) return holder;
            if (iter->second.expired()) img_table_.erase(iter);
        }
        if (auto holder = img_table_[key].lock()) return holder;
        auto built = std::shared_ptr<Image>(new Image{key, pixels});
        img_table_[key] = built;
        return built;
    }

    std::size_t size() const {
        std::size_t alive_cnt = 0;
        for (auto& row : img_table_)
            if (!row.second.expired()) ++alive_cnt;
        return alive_cnt;
    }

    void drop_all() { img_table_.clear(); }

    bool alive(const std::string& key, int& pixels_out) const {
        auto iter = img_table_.find(key);
        if (iter == img_table_.end()) return false;
        if (auto holder = iter->second.lock()) {
            pixels_out = holder->pixel_count;
            return true;
        }
        return false;
    }
};

int main() {
    ImageCache mgr;
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "LOAD") {
            std::string asset_id;
            int px_val;
            std::cin >> asset_id >> px_val;
            {
                std::shared_ptr<Image> pic = mgr.load(asset_id, px_val);
                std::cout << "loaded " << pic->name << " pixels=" << pic->pixel_count << '\n';
            }
        } else if (word == "QUERY") {
            std::string asset_id;
            std::cin >> asset_id;
            int px_val;
            if (mgr.alive(asset_id, px_val)) std::cout << "pixels=" << px_val << std::endl;
            else std::cout << "EXPIRED\n";
        } else if (word == "DROP_ALL") {
            mgr.drop_all();
        }
    }
    return 0;
}
