#include <vector>
#include <utility>
#include <string>
#include <iostream>
// adapted from classmate submission

class StringBuilder {
    std::string buf_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : buf_(std::move(o.buf_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        buf_ = std::move(o.buf_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) { buf_ += s; return *this; }
    StringBuilder& append(std::string&& s) { buf_ += std::move(s); return *this; }
    StringBuilder& append_line(const std::string& s) { return append(s).append("\n"); }
    const std::string& str() const { return buf_; }
    void clear() { buf_.clear(); }
};

int main()
{
    StringBuilder document;
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string name;
        std::getline(std::cin, name);
        name.erase(0, name.find_first_not_of(' '));

        std::vector<std::string> lines;
        std::string ln;
        while (std::getline(std::cin, ln)) {
            if (ln.empty()) break;
            lines.push_back(ln);
        }

        StringBuilder block;
        block.append("# ").append(name).append_line("");
        for (const auto& l : lines) block.append_line(l);
        block.append_line("");

        document.append(std::move(block.str()));
    }

    std::cout << document.str();
    return 0;
}
