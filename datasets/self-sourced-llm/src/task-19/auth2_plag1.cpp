#include <iostream>
#include <sstream>
#include <string>
#include <utility>
// cleaned up version

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : content_(std::move(o.content_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        content_ = std::move(o.content_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        content_ += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        content_ += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        content_.append(s);
        content_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return content_; }
    void clear() { content_.clear(); }

private:
    std::string content_;
};

int main() {
    StringBuilder output;

    for (std::string token; std::cin >> token;) {
        if (token == "END") break;
        if (token != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        if (!heading.empty() && heading[0] == ' ') heading.erase(0, 1);

        std::ostringstream block;
        block << "# " << heading << std::endl;

        std::string s;
        while (std::getline(std::cin, s)) {
            if (s.empty()) break;
            block << s << std::endl;
        }
        block << std::endl;

        output.append(std::move(block.str()));
    }

    std::cout << output.str();
    return 0;
}
