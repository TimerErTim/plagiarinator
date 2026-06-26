#include <iostream>
#include <string>
#include <utility>
#include <vector>

class StringBuilder {
    std::string text_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : text_(std::move(o.text_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        text_ = std::move(o.text_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) { text_ += s; return *this; }
    StringBuilder& append(std::string&& s) { text_ += std::move(s); return *this; }
    StringBuilder& append_line(const std::string& s) { return append(s).append("\n"); }
    const std::string& str() const { return text_; }
    void clear() { text_.clear(); }
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
        std::string ln;
        while (std::getline(std::cin, ln)) {
            if (ln.empty()) break;
            lines.push_back(ln);
        }

        StringBuilder section;
        section.append("# ").append(title).append_line("");
        for (const auto& l : lines) section.append_line(l);
        section.append_line("");

        doc.append(std::move(section.str()));
    }

    std::cout << doc.str();
    return 0;
}

#include <vector>
