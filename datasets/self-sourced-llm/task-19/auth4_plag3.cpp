#include <utility>
#include <iostream>
#include <string>
// TODO: refactor later

class StringBuilder {
public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& other) noexcept;
    StringBuilder& operator=(StringBuilder&& other) noexcept;
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s);
    StringBuilder& append(std::string&& s);
    StringBuilder& append_line(const std::string& s);
    const std::string& str() const { return pool_; }
    void clear() { pool_.clear(); }

private:
    std::string pool_;
};

StringBuilder::StringBuilder(StringBuilder&& other) noexcept : pool_(std::move(other.pool_)) {
    other.pool_.clear();
}

StringBuilder& StringBuilder::operator=(StringBuilder&& other) noexcept {
    pool_ = std::move(other.pool_);
    other.pool_.clear();
    return *this;
}

StringBuilder& StringBuilder::append(const std::string& s) {
    pool_ += s;
    return *this;
}

StringBuilder& StringBuilder::append(std::string&& s) {
    pool_ += std::move(s);
    return *this;
}

StringBuilder& StringBuilder::append_line(const std::string& s) {
    pool_ += s;
    pool_ += '\n';
    return *this;
}

int main() {
    StringBuilder output;
    std::string ln;

    while (std::cin >> ln) {
        if (ln == "END") break;
        if (ln != "SECTION") continue;

        std::string name;
        std::getline(std::cin, name);
        while (!name.empty() && name[0] == ' ') name.erase(0, 1);

        StringBuilder chunk;
        chunk.append("# ").append(name).append("\n");

        std::string buf;
        while (std::getline(std::cin, buf) && !buf.empty())
            chunk.append_line(buf);
        chunk.append("\n");

        output.append(std::move(chunk.str()));
    }

    std::cout << output.str();
    return 0;
}
