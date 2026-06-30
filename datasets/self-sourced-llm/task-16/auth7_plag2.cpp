#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Image {
    std::string name;
    int pixel_count;
};

class ImageCache {
    std::unordered_map<std::string, std::weak_ptr<Image>> slots_;

    std::shared_ptr<Image> find_alive(const std::string& key) const
    {
        auto it = slots_.find(key);
        if (it == slots_.end()) return {};
        return it->second.lock();
    }

    std::shared_ptr<Image> insert_new(const std::string& key, int pixels)
    {
        auto img = std::shared_ptr<Image>(new Image{key, pixels});
        slots_[key] = img;
        return img;
    }

public:
    std::shared_ptr<Image> load(const std::string& key, int pixels)
    {
        if (auto sp = find_alive(key)) return sp;
        return insert_new(key, pixels);
    }

    std::size_t size() const
    {
        std::size_t n = 0;
        for (const auto& kv : slots_)
            if (!kv.second.expired()) ++n;
        return n;
    }

    void drop_all() { slots_.clear(); }

    bool query(const std::string& key, int& pixels) const
    {
        std::shared_ptr<Image> img = find_alive(key);
        if (!img) return false;
        pixels = img->pixel_count;
        return true;
    }
};

int main()
{
    ImageCache c;
    std::string op;
    while (std::cin >> op)
    {
        if (op == "END") break;
        if (op == "LOAD")
        {
            std::string k;
            int v;
            std::cin >> k >> v;
            {
                auto im = c.load(k, v);
                std::cout << "loaded " << im->name << " pixels=" << im->pixel_count << '\n';
            }
        }
        else if (op == "QUERY")
        {
            std::string k;
            std::cin >> k;
            int px;
            if (c.query(k, px)) std::cout << "pixels=" << px << '\n';
            else std::cout << "EXPIRED\n";
        }
        else if (op == "DROP_ALL")
        {
            c.drop_all();
        }
    }
    return 0;
}
