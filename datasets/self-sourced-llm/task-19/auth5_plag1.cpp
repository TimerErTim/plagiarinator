#include <iostream>
#include <string>
#include <utility>
// cleaned up version

class StringBuilder {
    std::string storage;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& from) noexcept : storage(std::move(from.storage)) {}
    StringBuilder& operator=(StringBuilder&& from) noexcept {
        storage = std::move(from.storage);
        from.storage.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& text) {
        storage += text;
        return *this;
    }

    StringBuilder& append(std::string&& text) {
        storage += std::move(text);
        return *this;
    }

    StringBuilder& append_line(const std::string& text) {
        storage.append(text).push_back('\n');
        return *this;
    }

    const std::string& str() const { return storage; }
    void clear() { storage.clear(); }
};

int main() {
    StringBuilder output;
    std::string label;

    while (std::cin >> label) {
        if (label == "END") break;
        if (label != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        if (heading.size() && heading[0] == ' ') heading = heading.substr(1);

        std::string body;
        body.reserve(128);
        body += "# ";
        body += heading;
        body += "\n";

        std::string s;
        while (std::getline(std::cin, s)) {
            if (s.empty()) break;
            body += s;
            body += '\n';
        }
        body += '\n';

        output.append(std::move(body));
    }

    std::cout << output.str();
    return 0;
}
