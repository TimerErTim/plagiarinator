#include <string>
#include <utility>
#include <iostream>

class StringBuilder {
    std::string buf_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& src) noexcept : buf_(std::move(src.buf_)) {}
    StringBuilder& operator=(StringBuilder&& src) noexcept {
        buf_ = std::move(src.buf_);
        src.buf_.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        buf_.append(s);
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        buf_.append(std::move(s));
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        buf_.append(s);
        buf_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return buf_; }
    void clear() { buf_.clear(); }
};

int main() {
    StringBuilder final_doc;
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        while (!heading.empty() && heading.front() == ' ') heading.erase(0, 1);

        std::string assembled;
        assembled += "# ";
        assembled += heading;
        assembled += "\n";

        std::string row;
        while (std::getline(std::cin, row)) {
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
