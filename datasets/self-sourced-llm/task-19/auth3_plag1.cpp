#include <iostream>
#include <string>
#include <utility>
// cleaned up version

class StringBuilder {
    std::string content_;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& rhs) noexcept : content_(std::exchange(rhs.content_, {})) {}

    StringBuilder& operator=(StringBuilder&& rhs) noexcept {
        if (this != &rhs) content_ = std::exchange(rhs.content_, {});
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& chunk) {
        content_.append(chunk);
        return *this;
    }

    StringBuilder& append(std::string&& chunk) {
        content_.append(std::move(chunk));
        return *this;
    }

    StringBuilder& append_line(const std::string& line) {
        append(line);
        content_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return content_; }

    void clear() { content_.clear(); }
};

static StringBuilder read_section(std::istream& in) {
    std::string title_line;
    std::getline(in, title_line);
    if (!title_line.empty() && title_line.front() == ' ')
        title_line.erase(0, 1);

    StringBuilder part;
    part.append("# ").append(title_line).append_line("");

    std::string content;
    while (std::getline(in, content)) {
        if (content.empty()) break;
        part.append_line(content);
    }
    part.append_line("");
    return part;
}

int main() {
    StringBuilder full;
    std::string token;

    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "SECTION") {
            StringBuilder sec = read_section(std::cin);
            full.append(std::move(sec.str()));
        }
    }

    std::cout << full.str();
    return 0;
}
