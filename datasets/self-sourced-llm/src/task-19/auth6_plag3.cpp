#include <vector>
#include <iostream>
#include <string>
#include <utility>
// TODO: refactor later

class StringBuilder {
    std::string body_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : body_(std::move(o.body_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        body_ = std::move(o.body_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) { body_ += s; return *this; }
    StringBuilder& append(std::string&& s) { body_ += std::move(s); return *this; }
    StringBuilder& append_line(const std::string& s) { return append(s).append("\n"); }
    const std::string& str() const { return body_; }
    void clear() { body_.clear(); }
};

int main() {
    StringBuilder text;
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

        StringBuilder seg;
        seg.append("# ").append(hdr).append_line("");
        for (const auto& l : lines) seg.append_line(l);
        seg.append_line("");

        text.append(std::move(seg.str()));
    }

    std::cout << text.str();
    return 0;
}
