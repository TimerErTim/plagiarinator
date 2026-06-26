#include <utility>
#include <string>
#include <sstream>
#include <iostream>
// adapted from classmate submission

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : buffer_(std::move(o.buffer_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        buffer_ = std::move(o.buffer_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        buffer_ += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        buffer_ += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        buffer_.append(s);
        buffer_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return buffer_; }
    void clear() { buffer_.clear(); }

private:
    std::string buffer_;
};

int main()
{
    StringBuilder document;

    for (std::string op; std::cin >> op;) {
        if (op == "END") break;
        if (op != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        if (!heading.empty() && heading[0] == ' ') heading.erase(0, 1);

        std::ostringstream block;
        block << "# " << heading << "\n";

        std::string input;
        while (std::getline(std::cin, input)) {
            if (input.empty()) break;
            block << input << '\n';
        }
        block << '\n';

        document.append(std::move(block.str()));
    }

    std::cout << document.str();
    return 0;
}
