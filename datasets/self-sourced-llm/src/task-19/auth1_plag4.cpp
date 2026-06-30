#include <iostream>
#include <string>
#include <utility>

class StringBuilder {
    std::string data;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : data(std::move(other.data)) {}

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        if (this != &other) data = std::move(other.data);
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        data += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        data += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        data += s;
        data += '\n';
        return *this;
    }

    const std::string& str() const { return data; }

    void clear() { data.clear(); }
};

int main() {
    StringBuilder doc;
    std::string tok;

    while (std::cin >> tok) {
        if (tok == "END") break;
        if (tok != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        title.erase(0, title.find_first_not_of(' '));

        StringBuilder section;
        section.append("# ").append(title).append_line("");

        std::string ln;
        while (std::getline(std::cin, ln)) {
            if (ln.empty()) break;
            section.append_line(ln);
        }
        section.append_line("");
        doc.append(std::move(section.str()));
    }

    std::cout << doc.str();
    return 0;
}
