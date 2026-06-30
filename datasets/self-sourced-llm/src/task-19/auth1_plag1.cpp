#include <string>
#include <utility>
#include <iostream>

class StringBuilder {
    std::string buf;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : buf(std::move(other.buf)) {}

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        if (this != &other) buf = std::move(other.buf);
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        buf += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        buf += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        buf += s;
        buf.push_back('\n');
        return *this;
    }

    const std::string& str() const { return buf; }

    void clear() { buf.clear(); }
};

int main() {
    StringBuilder output;
    std::string word;

    for (; std::cin >> word;) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        title.erase(0, title.find_first_not_of(' '));

        StringBuilder block;
        block.append("# ").append(title).append_line("");

        std::string input;
        while (std::getline(std::cin, input)) {
            if (input.empty()) break;
            block.append_line(input);
        }
        block.append_line("");
        output.append(std::move(block.str()));
    }

    std::cout << output.str();
    return 0;
}
