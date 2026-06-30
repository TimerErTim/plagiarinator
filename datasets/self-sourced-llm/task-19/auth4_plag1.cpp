#include <string>
#include <utility>
#include <iostream>

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& other) noexcept;
    StringBuilder& operator=(StringBuilder&& other) noexcept;
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s);
    StringBuilder& append(std::string&& s);
    StringBuilder& append_line(const std::string& s);
    const std::string& str() const { return content_; }
    void clear() { content_.clear(); }

private:
    std::string content_;
};

StringBuilder::StringBuilder(StringBuilder&& other) noexcept : content_(std::move(other.content_)) {
    other.content_.clear();
}

StringBuilder& StringBuilder::operator=(StringBuilder&& other) noexcept {
    content_ = std::move(other.content_);
    other.content_.clear();
    return *this;
}

StringBuilder& StringBuilder::append(const std::string& s) {
    content_ += s;
    return *this;
}

StringBuilder& StringBuilder::append(std::string&& s) {
    content_ += std::move(s);
    return *this;
}

StringBuilder& StringBuilder::append_line(const std::string& s) {
    content_ += s;
    content_.push_back('\n');
    return *this;
}

int main() {
    StringBuilder output;
    std::string cmd;

    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd != "SECTION") continue;

        std::string name;
        std::getline(std::cin, name);
        while (!name.empty() && name[0] == ' ') name.erase(0, 1);

        StringBuilder chunk;
        chunk.append("# ").append(name).append("\n");

        std::string line;
        while (std::getline(std::cin, line) && !line.empty())
            chunk.append_line(line);
        chunk.append("\n");

        output.append(std::move(chunk.str()));
    }

    std::cout << output.str();
    return 0;
}
