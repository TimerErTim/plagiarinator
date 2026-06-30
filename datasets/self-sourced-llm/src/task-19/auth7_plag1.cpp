#include <utility>
#include <iostream>
#include <string>

class StringBuilder {
public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : buf_(std::move(other.buf_)) {
        other.buf_.clear();
    }

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        buf_ = std::move(other.buf_);
        other.buf_.clear();
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& frag) {
        buf_.append(frag);
        return *this;
    }

    StringBuilder& append(std::string&& frag) {
        buf_.append(std::move(frag));
        return *this;
    }

    StringBuilder& append_line(const std::string& frag) {
        buf_.append(frag);
        buf_.append(1, '\n');
        return *this;
    }

    const std::string& str() const { return buf_; }
    void clear() { buf_.clear(); }

private:
    std::string buf_;
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
    std::string verb;

    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "SECTION") consume_section(document, std::cin);
    }

    std::cout << document.str();
    return 0;
}
