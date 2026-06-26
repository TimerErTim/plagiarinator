#include <iostream>
#include <string>
#include <utility>

class StringBuilder {
    std::string buffer_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& src) noexcept : buffer_(std::move(src.buffer_)) {}
    StringBuilder& operator=(StringBuilder&& src) noexcept {
        buffer_ = std::move(src.buffer_);
        src.buffer_.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        buffer_.append(s);
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        buffer_.append(std::move(s));
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        buffer_.append(s);
        buffer_.append(1, '\n');
        return *this;
    }

    const std::string& str() const { return buffer_; }
    void clear() { buffer_.clear(); }
};

int main() {
    StringBuilder final_doc;
    std::string token;

    while (std::cin >> token) {
        if (token == "END") break;
        if (token != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        while (!heading.empty() && heading.front() == ' ') heading.erase(0, 1);

        std::string assembled;
        assembled += "# ";
        assembled += heading;
        assembled += "\n";

        for (std::string row; std::getline(std::cin, row);) {
            if (row.empty()) break;
            assembled += row;
            assembled += '\n';
        }
        assembled += '\n';

        StringBuilder tmp;
        tmp.append(std::move(assembled));
        final_doc.append(std::move(tmp.str()));
    }

    std::cout << final_doc.str();
    return 0;
}
