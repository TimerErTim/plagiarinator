#include <string>
#include <utility>
#include <iostream>

class StringBuilder {
    std::string buf;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& from) noexcept : buf(std::move(from.buf)) {}
    StringBuilder& operator=(StringBuilder&& from) noexcept {
        buf = std::move(from.buf);
        from.buf.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& chunk) {
        buf += chunk;
        return *this;
    }

    StringBuilder& append(std::string&& chunk) {
        buf += std::move(chunk);
        return *this;
    }

    StringBuilder& append_line(const std::string& chunk) {
        buf.append(chunk).push_back('\n');
        return *this;
    }

    const std::string& str() const { return buf; }
    void clear() { buf.clear(); }
};

int main() {
    StringBuilder master;
    std::string cmd;

    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        if (title.size() && title[0] == ' ') title = title.substr(1);

        std::string body;
        body.reserve(128);
        body += "# ";
        body += title;
        body += "\n";

        std::string entry;
        while (std::getline(std::cin, entry)) {
            if (entry.empty()) break;
            body += entry;
            body += '\n';
        }
        body += '\n';

        master.append(std::move(body));
    }

    std::cout << master.str();
    return 0;
}
