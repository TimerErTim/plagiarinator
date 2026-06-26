#include <iostream>
#include <string>
#include <utility>
// cleaned up version

class StringBuilder {
public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : content_(std::move(other.content_)) {
        other.content_.clear();
    }

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        content_ = std::move(other.content_);
        other.content_.clear();
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& piece) {
        content_.append(piece);
        return *this;
    }

    StringBuilder& append(std::string&& piece) {
        content_.append(std::move(piece));
        return *this;
    }

    StringBuilder& append_line(const std::string& piece) {
        content_.append(piece);
        content_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return content_; }
    void clear() { content_.clear(); }

private:
    std::string content_;
};

void consume_section(StringBuilder& doc, std::istream& in) {
    std::string title;
    std::getline(in, title);
    if (!title.empty() && title[0] == ' ') title.erase(0, 1);

    StringBuilder local;
    local.append("# ").append(title).append("\n");

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) break;
        local.append_line(line);
    }
    local.append("\n");
    doc.append(std::move(local.str()));
}

int main() {
    StringBuilder output;
    std::string token;

    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "SECTION") consume_section(output, std::cin);
    }

    std::cout << output.str();
    return 0;
}
