#include <string>
#include <iostream>
#include <utility>
// quick submit

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
    const std::string& str() const { return data_; }
    void clear() { data_.clear(); }

private:
    std::string data_;
};

StringBuilder::StringBuilder(StringBuilder&& other) noexcept : data_(std::move(other.data_)) {
    other.data_.clear();
}

StringBuilder& StringBuilder::operator=(StringBuilder&& other) noexcept {
    data_ = std::move(other.data_);
    other.data_.clear();
    return *this;
}

StringBuilder& StringBuilder::append(const std::string& s) {
    data_ += s;
    return *this;
}

StringBuilder& StringBuilder::append(std::string&& s) {
    data_ += std::move(s);
    return *this;
}

StringBuilder& StringBuilder::append_line(const std::string& s) {
    data_ += s;
    data_ += '\n';
    return *this;
}

int main() {
    StringBuilder output;
    std::string word;

    while (std::cin >> word) {
        if (word == "END") break;
        if (word != "SECTION") continue;

        std::string name;
        std::getline(std::cin, name);
        while (!name.empty() && name[0] == ' ') name.erase(0, 1);

        StringBuilder chunk;
        chunk.append("# ").append(name).append("\n");

        std::string str;
        while (std::getline(std::cin, str) && !str.empty())
            chunk.append_line(str);
        chunk.append("\n");

        output.append(std::move(chunk.str()));
    }

    std::cout << output.str();
    return 0;
}
