#include <iostream>
#include <string>
#include <utility>

class StringBuilder {
public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : payload_(std::move(other.payload_)) {
        other.payload_.clear();
    }

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        payload_ = std::move(other.payload_);
        other.payload_.clear();
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& piece) {
        payload_.append(piece);
        return *this;
    }

    StringBuilder& append(std::string&& piece) {
        payload_.append(std::move(piece));
        return *this;
    }

    StringBuilder& append_line(const std::string& piece) {
        payload_.append(piece);
        payload_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return payload_; }
    void clear() { payload_.clear(); }

private:
    std::string payload_;
};

void consume_section(StringBuilder& doc, std::istream& in) {
    std::string title;
    std::getline(in, title);
    if (!title.empty() && title[0] == ' ') title.erase(0, 1);

    StringBuilder local;
    local.append("# ").append(title).append("\n");

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) break;
        local.append_line(line);
    }
    local.append("\n");
    doc.append(std::move(local.str()));
}

int main() {
    StringBuilder document;
    std::string cmd;

    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "SECTION") consume_section(document, std::cin);
    }

    std::cout << document.str();
    return 0;
}
