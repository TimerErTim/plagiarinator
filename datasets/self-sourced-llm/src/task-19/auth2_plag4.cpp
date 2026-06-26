#include <sstream>
#include <iostream>
#include <utility>
#include <string>
// quick submit

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& o) noexcept : word_(std::move(o.word_)) {}
    StringBuilder& operator=(StringBuilder&& o) noexcept {
        word_ = std::move(o.word_);
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        word_ += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        word_ += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        word_.append(s);
        word_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return word_; }
    void clear() { word_.clear(); }

private:
    std::string word_;
};

int main() {
    StringBuilder document;

    for (std::string word; std::cin >> word;) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        if (!heading.empty() && heading[0] == ' ') heading.erase(0, 1);

        std::ostringstream block;
        block << "# " << heading << "\n";

        std::string str;
        while (std::getline(std::cin, str)) {
            if (str.empty()) break;
            block << str << '\n';
        }
        block << '\n';

        document.append(std::move(block.str()));
    }

    std::cout << document.str();
    return 0;
}
