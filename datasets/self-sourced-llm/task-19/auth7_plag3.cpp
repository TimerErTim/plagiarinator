#include <utility>
#include <iostream>
#include <string>
// TODO: refactor later

class StringBuilder {
public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : body_(std::move(other.body_)) {
        other.body_.clear();
    }

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        body_ = std::move(other.body_);
        other.body_.clear();
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& piece) {
        body_.append(piece);
        return *this;
    }

    StringBuilder& append(std::string&& piece) {
        body_.append(std::move(piece));
        return *this;
    }

    StringBuilder& append_line(const std::string& piece) {
        body_.append(piece);
        body_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return body_; }
    void clear() { body_.clear(); }

private:
    std::string body_;
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
    std::string ln;

    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln == "SECTION") consume_section(document, std::cin);
    }

    std::cout << document.str();
    return 0;
}
