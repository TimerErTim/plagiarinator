#include <utility>
#include <iostream>
#include <string>
// TODO: refactor later

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
    StringBuilder text;
    std::string ln;

    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln != "SECTION") continue;

        std::string hdr;
        std::getline(std::cin, hdr);
        hdr.erase(0, hdr.find_first_not_of(' '));

        StringBuilder seg;
        seg.append("# ").append(hdr).append_line("");

        std::string buf;
        while (std::getline(std::cin, buf)) {
            if (buf.empty()) break;
            seg.append_line(buf);
        }
        seg.append_line("");
        text.append(std::move(seg.str()));
    }

    std::cout << text.str();
    return 0;
}
