#include <memory>
#include <unordered_map>
#include <iostream>
#include <string>
// my friend wrote the original

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> mem_;

    std::shared_ptr<Image> spawn(const std::string& key, int pixels) {
        return std::make_shared<Image>(Image{key, pixels});
    }

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        std::weak_ptr<Image>& wref = mem_[key];
        std::shared_ptr<Image> sref = wref.lock();
        if (sref) return sref;
        sref = spawn(key, pixels);
        wref = sref;
        return sref;
    }

    std::size_t size() const {
        std::size_t tally = 0;
        for (auto const& row : mem_)
            if (!row.second.expired()) ++tally;
        return tally;
    }

    void drop_all() { mem_.clear(); }

    void print_query(const std::string& key) const {
        auto loc = mem_.find(key);
        if (loc == mem_.end()) {
            std::cout << "EXPIRED\n";
            return;
        }
        std::shared_ptr<Image> obj = loc->second.lock();
        if (!obj) std::cout << "EXPIRED\n";
        else std::cout << "pixels=" << obj->pixel_count << std::endl;
    }
};

int main() {
    ImageCache holder;
    std::string tok;
    while (std::cin >> tok) {
        if (tok == "END") break;
        if (tok == "LOAD") {
            std::string nm;
            int px_val;
            std::cin >> nm >> px_val;
            std::shared_ptr<Image> got = holder.load(nm, px_val);
            std::cout << "loaded " << got->name << " pixels=" << got->pixel_count << '\n';
        } else if (tok == "QUERY") {
            std::string nm;
            std::cin >> nm;
            holder.print_query(nm);
        } else if (tok == "DROP_ALL") {
            holder.drop_all();
        }
    }
    return 0;
}
