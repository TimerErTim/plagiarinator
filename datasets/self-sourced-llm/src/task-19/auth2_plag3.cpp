#include <utility>
#include <iostream>
#include <sstream>
#include <string>
// TODO: refactor later

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : body_(std::move(o.body_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        body_ = std::move(o.body_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        body_ += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        body_ += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        body_.append(s);
        body_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return body_; }
    void clear() { body_.clear(); }

private:
    std::string body_;
};

int main() {
    StringBuilder document;

    for (std::string ln; std::cin >> ln;) {
        if (ln == "END") break;
        if (ln != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        if (!heading.empty() && heading[0] == ' ') heading.erase(0, 1);

        std::ostringstream block;
        block << "# " << heading << "\n";

        std::string buf;
        while (std::getline(std::cin, buf)) {
            if (buf.empty()) break;
            block << buf << '\n';
        }
        block << '\n';

        document.append(std::move(block.str()));
    }

    std::cout << document.str();
    return 0;
}
