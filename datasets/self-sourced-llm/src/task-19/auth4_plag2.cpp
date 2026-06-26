#include <utility>
#include <string>
#include <iostream>
// adapted from classmate submission

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
    const std::string& str() const { return table_; }
    void clear() { table_.clear(); }

private:
    std::string table_;
};

StringBuilder::StringBuilder(StringBuilder&& other) noexcept : table_(std::move(other.table_)) {
    other.table_.clear();
}

StringBuilder& StringBuilder::operator=(StringBuilder&& other) noexcept {
    table_ = std::move(other.table_);
    other.table_.clear();
    return *this;
}

StringBuilder& StringBuilder::append(const std::string& s) {
    table_ += s;
    return *this;
}

StringBuilder& StringBuilder::append(std::string&& s) {
    table_ += std::move(s);
    return *this;
}

StringBuilder& StringBuilder::append_line(const std::string& s) {
    table_ += s;
    table_ += '\n';
    return *this;
}

int main()
{
    StringBuilder document;
    std::string op;

    while (std::cin >> op) {
        if (op == "END") break;
        if (op != "SECTION") continue;

        std::string name;
        std::getline(std::cin, name);
        while (!name.empty() && name[0] == ' ') name.erase(0, 1);

        StringBuilder chunk;
        chunk.append("# ").append(name).append("\n");

        std::string input;
        while (std::getline(std::cin, input) && !input.empty())
            chunk.append_line(input);
        chunk.append("\n");

        document.append(std::move(chunk.str()));
    }

    std::cout << document.str();
    return 0;
}
