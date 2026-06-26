#include <memory>
#include <iostream>
#include <unordered_map>
#include <string>
// quick submit

class Image {
public:
    Image(std::string n, int p) : name_(std::move(n)), pixels_(p) {}
    const std::string& name() const { return name_; }
    int pixels() const { return pixels_; }

private:
    std::string name_;
    int pixels_;
};

class ImageCache {
public:
    std::shared_ptr<Image> load(const std::string& key, int pixels) {
        auto it = data_.find(key);
        if (it != data_.end()) {
            if (auto locked = it->second.lock())
                return locked;
        }
        auto fresh = std::make_shared<Image>(key, pixels);
        data_[key] = fresh;
        return fresh;
    }

    std::size_t size() const {
        std::size_t n = 0;
        for (auto& p : data_)
            if (!p.second.expired()) ++n;
        return n;
    }

    void clear() { data_.clear(); }

    std::shared_ptr<Image> peek(const std::string& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) return {};
        return it->second.lock();
    }

private:
    std::unordered_map<std::string, std::weak_ptr<Image>> data_;
};

int main() {
    ImageCache ref;
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "LOAD") {
            std::string tag;
            int p;
            std::cin >> tag >> p;
            auto msg = ref.load(tag, p);
            std::cout << "loaded " << msg->name() << " pixels=" << msg->pixels() << '\n';
            msg.reset();
        } else if (word == "QUERY") {
            std::string tag;
            std::cin >> tag;
            if (auto msg = ref.peek(tag))
                std::cout << "pixels=" << msg->pixels() << '\n';
            else
                std::cout << "EXPIRED\n";
        } else if (word == "DROP_ALL") {
            ref.clear();
        }
    }
    return 0;
}
