#include <iostream>
#include <string>
#include <utility>
// cleaned up version

class StringBuilder {
    std::string content_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& src) noexcept : content_(std::move(src.content_)) {}
    StringBuilder& operator=(StringBuilder&& src) noexcept {
        content_ = std::move(src.content_);
        src.content_.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        content_.append(s);
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        content_.append(std::move(s));
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        content_.append(s);
        content_.append(1, '\n');
        return *this;
    }

    const std::string& str() const { return content_; }
    void clear() { content_.clear(); }
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

        for (std::string s; std::getline(std::cin, s);) {
            if (s.empty()) break;
            assembled += s;
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
