#include <iostream>
#include <string>
#include <utility>

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
    StringBuilder doc;
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string title;
        std::getline(std::cin, title);
        if (title.size() && title[0] == ' ') title = title.substr(1);

        std::string body;
        body.reserve(128);
        body += "# ";
        body += title;
        body += "\n";

        std::string row;
        while (std::getline(std::cin, row)) {
            if (row.empty()) break;
            body += row;
            body += '\n';
        }
        body += '\n';

        doc.append(std::move(body));
    }

    std::cout << doc.str();
    return 0;
}
