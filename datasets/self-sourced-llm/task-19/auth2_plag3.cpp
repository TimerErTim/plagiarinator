#include <sstream>
#include <iostream>
#include <string>
#include <utility>
// borrowed from roommate's homework

class StringBuilder
{
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : heap_(std::move(o.heap_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept
    {
        heap_ = std::move(o.heap_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s)
    {
        heap_ += s;
        return *this;
    }

    StringBuilder& append(std::string&& s)
    {
        heap_ += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s)
    {
        heap_.append(s);
        heap_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return heap_; }
    void clear() { heap_.clear(); }

private:
    std::string heap_;
};

int main()
{
    StringBuilder document;

    for (std::string opcode; std::cin >> opcode;)
    {
        if (opcode == "END") break;
        if (opcode != "SECTION") continue;

        std::string hdr;
        std::getline(std::cin, hdr);
        if (!hdr.empty() && hdr[0] == ' ') hdr.erase(0, 1);

        std::ostringstream oss;
        oss << "# " << hdr << std::endl;

        std::string raw;
        while (std::getline(std::cin, raw))
        {
            if (raw.empty()) break;
            oss << raw << std::endl;
        }
        oss << std::endl;

        document.append(std::move(oss.str()));
    }

    std::cout << document.str();
    return 0;
}
