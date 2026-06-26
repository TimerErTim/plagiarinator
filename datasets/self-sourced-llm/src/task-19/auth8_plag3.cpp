#include <utility>
#include <iostream>
#include <string>
// TODO: refactor later

class StringBuilder {
    std::string body_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& src) noexcept : body_(std::move(src.body_)) {}
    StringBuilder& operator=(StringBuilder&& src) noexcept {
        body_ = std::move(src.body_);
        src.body_.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        body_.append(s);
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        body_.append(std::move(s));
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        body_.append(s);
        body_.append(1, '\n');
        return *this;
    }

    const std::string& str() const { return body_; }
    void clear() { body_.clear(); }
};

int main() {
    StringBuilder final_doc;
    std::string ln;

    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        while (!heading.empty() && heading.front() == ' ') heading.erase(0, 1);

        std::string assembled;
        assembled += "# ";
        assembled += heading;
        assembled += "\n";

        for (std::string buf; std::getline(std::cin, buf);) {
            if (buf.empty()) break;
            assembled += buf;
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
