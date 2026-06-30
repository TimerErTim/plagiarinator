#include <vector>
#include <iostream>
#include <string>
#include <utility>

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

int main() {
    StringBuilder doc;
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        title.erase(0, title.find_first_not_of(' '));

        std::vector<std::string> lines;
        std::string row;
        while (std::getline(std::cin, row)) {
            if (row.empty()) break;
            lines.emplace_back(row);
        }

        StringBuilder section;
        section.append("# ").append(title).append_line("");
        for (std::size_t i = 0; i < lines.size(); ++i)
            section.append_line(lines[i]);
        section.append_line("");

        doc.append(std::move(section.str()));
    }

    std::cout << doc.str();
    return 0;
}
