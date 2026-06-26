#include <string>
#include <iostream>
#include <utility>
// quick submit

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
    StringBuilder ref;
    std::string elem;

    while (std::cin >> elem) {
        if (elem == "END") break;
        if (elem != "SECTION") continue;

        std::string hdr;
        std::getline(std::cin, hdr);
        hdr.erase(0, hdr.find_first_not_of(' '));

        StringBuilder tmp;
        tmp.append("# ").append(hdr).append_line("");

        std::string str;
        while (std::getline(std::cin, str)) {
            if (str.empty()) break;
            tmp.append_line(str);
        }
        tmp.append_line("");
        ref.append(std::move(tmp.str()));
    }

    std::cout << ref.str();
    return 0;
}
