#include <iostream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class StringBuilder {
    std::string content_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : content_(std::move(o.content_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        content_ = std::move(o.content_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) { content_ += s; return *this; }
    StringBuilder& append(std::string&& s) { content_ += std::move(s); return *this; }
    StringBuilder& append_line(const std::string& s) { return append(s).append("\n"); }
    const std::string& str() const { return content_; }
    void clear() { content_.clear(); }
};

int main() {
    StringBuilder output;
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        heading.erase(0, heading.find_first_not_of(' '));

        std::vector<std::string> lines;
        std::string ln;
        while (std::getline(std::cin, ln)) {
            if (ln.empty()) break;
            lines.push_back(ln);
        }

        StringBuilder part;
        part.append("# ").append(heading).append_line("");
        for (const auto& l : lines) part.append_line(l);
        part.append_line("");

        output.append(std::move(part.str()));
    }

    std::cout << output.str();
    return 0;
}
