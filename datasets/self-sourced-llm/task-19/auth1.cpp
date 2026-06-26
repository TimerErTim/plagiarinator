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
    std::string token;

    while (std::cin >> token) {
        if (token == "END") break;
        if (token != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        title.erase(0, title.find_first_not_of(' '));

        StringBuilder section;
        section.append("# ").append(title).append_line("");

        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) break;
            section.append_line(line);
        }
        section.append_line("");
        doc.append(std::move(section.str()));
    }

    std::cout << doc.str();
    return 0;
}
