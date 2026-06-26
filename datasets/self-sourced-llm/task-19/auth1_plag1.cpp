#include <iostream>
#include <string>
#include <utility>
// cleaned up version

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
    StringBuilder output;
    std::string token;

    while (std::cin >> token) {
        if (token == "END") break;
        if (token != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        heading.erase(0, heading.find_first_not_of(' '));

        StringBuilder part;
        part.append("# ").append(heading).append_line("");

        std::string s;
        while (std::getline(std::cin, s)) {
            if (s.empty()) break;
            part.append_line(s);
        }
        part.append_line("");
        output.append(std::move(part.str()));
    }

    std::cout << output.str();
    return 0;
}
