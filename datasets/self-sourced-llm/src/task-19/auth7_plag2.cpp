#include <utility>
#include <string>
#include <iostream>
// adapted from classmate submission

class StringBuilder {
public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : text_(std::move(other.text_)) {
        other.text_.clear();
    }

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        text_ = std::move(other.text_);
        other.text_.clear();
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& piece) {
        text_.append(piece);
        return *this;
    }

    StringBuilder& append(std::string&& piece) {
        text_.append(std::move(piece));
        return *this;
    }

    StringBuilder& append_line(const std::string& piece) {
        text_.append(piece);
        text_.push_back('\n');
        return *this;
    }

    const std::string& str() const { return text_; }
    void clear() { text_.clear(); }

private:
    std::string text_;
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

int main()
{
    StringBuilder document;
    std::string op;

    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "SECTION") consume_section(document, std::cin);
    }

    std::cout << document.str();
    return 0;
}
