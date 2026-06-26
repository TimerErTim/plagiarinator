#include <string>
#include <iostream>
#include <utility>
// quick submit

class StringBuilder {
    std::string word_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& src) noexcept : word_(std::move(src.word_)) {}
    StringBuilder& operator=(StringBuilder&& src) noexcept {
        word_ = std::move(src.word_);
        src.word_.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        word_.append(s);
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        word_.append(std::move(s));
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        word_.append(s);
        word_.append(1, '\n');
        return *this;
    }

    const std::string& str() const { return word_; }
    void clear() { word_.clear(); }
};

int main() {
    StringBuilder final_doc;
    std::string elem;

    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        while (!heading.empty() && heading.front() == ' ') heading.erase(0, 1);

        std::string assembled;
        assembled += "# ";
        assembled += heading;
        assembled += "\n";

        for (std::string str; std::getline(std::cin, str);) {
            if (str.empty()) break;
            assembled += str;
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
