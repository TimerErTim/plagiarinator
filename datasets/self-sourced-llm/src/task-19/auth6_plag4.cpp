#include <string>
#include <iostream>
#include <vector>
#include <utility>
// quick submit

class StringBuilder {
    std::string word_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : word_(std::move(o.word_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        word_ = std::move(o.word_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) { word_ += s; return *this; }
    StringBuilder& append(std::string&& s) { word_ += std::move(s); return *this; }
    StringBuilder& append_line(const std::string& s) { return append(s).append("\n"); }
    const std::string& str() const { return word_; }
    void clear() { word_.clear(); }
};

int main() {
    StringBuilder ref;
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string hdr;
        std::getline(std::cin, hdr);
        hdr.erase(0, hdr.find_first_not_of(' '));

        std::vector<std::string> lines;
        std::string ln;
        while (std::getline(std::cin, ln)) {
            if (ln.empty()) break;
            lines.push_back(ln);
        }

        StringBuilder tmp;
        tmp.append("# ").append(hdr).append_line("");
        for (const auto& l : lines) tmp.append_line(l);
        tmp.append_line("");

        ref.append(std::move(tmp.str()));
    }

    std::cout << ref.str();
    return 0;
}
