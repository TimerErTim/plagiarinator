#include <iostream>
#include <sstream>
#include <string>
#include <utility>

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : buf_(std::move(o.buf_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        buf_ = std::move(o.buf_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        buf_ += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        buf_ += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        buf_.append(s);
        buf_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return buf_; }
    void clear() { buf_.clear(); }

private:
    std::string buf_;
};

int main() {
    StringBuilder document;

    for (std::string cmd; std::cin >> cmd;) {
        if (cmd == "END") break;
        if (cmd != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        if (!heading.empty() && heading[0] == ' ') heading.erase(0, 1);

        std::ostringstream block;
        block << "# " << heading << "\n";

        std::string row;
        while (std::getline(std::cin, row)) {
            if (row.empty()) break;
            block << row << '\n';
        }
        block << '\n';

        document.append(std::move(block.str()));
    }

    std::cout << document.str();
    return 0;
}

#include <sstream>
