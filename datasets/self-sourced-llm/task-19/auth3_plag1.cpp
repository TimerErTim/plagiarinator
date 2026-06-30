#include <utility>
#include <iostream>
#include <string>

class StringBuilder {
    std::string store_;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& rhs) noexcept : store_(std::exchange(rhs.store_, {})) {}

    StringBuilder& operator=(StringBuilder&& rhs) noexcept {
        if (this != &rhs) store_ = std::exchange(rhs.store_, {});
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& piece) {
        store_.append(piece);
        return *this;
    }

    StringBuilder& append(std::string&& piece) {
        store_.append(std::move(piece));
        return *this;
    }

    StringBuilder& append_line(const std::string& line) {
        append(line);
        store_.emplace_back('\n');
        return *this;
    }

    const std::string& str() const { return store_; }

    void clear() { store_.clear(); }
};

static StringBuilder read_section(std::istream& in) {
    std::string title_line;
    std::getline(in, title_line);
    if (!title_line.empty() && title_line.front() == ' ')
        title_line.erase(0, 1);

    StringBuilder part;
    part.append("# ").append(title_line).append_line("");

    std::string line_buf;
    while (std::getline(in, line_buf)) {
        if (line_buf.empty()) break;
        part.append_line(line_buf);
    }
    part.append_line("");
    return part;
}

int main() {
    StringBuilder full;
    std::string key;

    while (std::cin >> key) {
        if (key == "END") break;
        if (key == "SECTION") {
            StringBuilder sec = read_section(std::cin);
            full.append(std::move(sec.str()));
        }
    }

    std::cout << full.str();
    return 0;
}
