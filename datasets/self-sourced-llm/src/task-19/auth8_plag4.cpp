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
    std::string tok;

    while (std::cin >> tok) {
        if (tok == "END") break;
        if (tok != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        while (!title.empty() && title.front() == ' ') title.erase(0, 1);

        std::string assembled;
        assembled += "# ";
        assembled += title;
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
