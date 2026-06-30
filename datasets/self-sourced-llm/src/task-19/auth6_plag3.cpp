#include <vector>
#include <utility>
#include <iostream>
#include <string>
// tweaked from lab partner code

class StringBuilder
{
    std::string store_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : store_(std::move(o.store_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept
    {
        store_ = std::move(o.store_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) { store_ += s; return *this; }
    StringBuilder& append(std::string&& s) { store_ += std::move(s); return *this; }
    StringBuilder& append_line(const std::string& s) { return append(s).append("\n"); }
    const std::string& str() const { return store_; }
    void clear() { store_.clear(); }
};

int main()
{
    StringBuilder rollup;
    std::string tok;

    while (std::cin >> tok)
    {
        if (tok == "END") break;
        if (tok != "SECTION") continue;

        std::string hdr;
        std::getline(std::cin, hdr);
        hdr.erase(0, hdr.find_first_not_of(' '));

        std::vector<std::string> rows;
        std::string ln;
        while (std::getline(std::cin, ln))
        {
            if (ln.empty()) break;
            rows.push_back(ln);
        }

        StringBuilder segment;
        segment.append("# ").append(hdr).append_line("");
        for (const auto& row : rows) segment.append_line(row);
        segment.append_line("");

        rollup.append(std::move(segment.str()));
    }

    std::cout << rollup.str();
    return 0;
}
