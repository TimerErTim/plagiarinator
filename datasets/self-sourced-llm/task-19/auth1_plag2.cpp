#include <utility>
#include <string>
#include <iostream>
// adapted from classmate submission

class StringBuilder {
    std::string data;

public:
    StringBuilder() = default;

    StringBuilder(StringBuilder&& other) noexcept : data(std::move(other.data)) {}

    StringBuilder& operator=(StringBuilder&& other) noexcept {
        if (this != &other) data = std::move(other.data);
        return *this;
    }

    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        data += s;
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        data += std::move(s);
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        data += s;
        data += '\n';
        return *this;
    }

    const std::string& str() const { return data; }

    void clear() { data.clear(); }
};

int main()
{
    StringBuilder document;
    std::string op;

    while (std::cin >> op) {
        if (op == "END") break;
        if (op != "SECTION") continue;

        std::string name;
        std::getline(std::cin, name);
        name.erase(0, name.find_first_not_of(' '));

        StringBuilder block;
        block.append("# ").append(name).append_line("");

        std::string input;
        while (std::getline(std::cin, input)) {
            if (input.empty()) break;
            block.append_line(input);
        }
        block.append_line("");
        document.append(std::move(block.str()));
    }

    std::cout << document.str();
    return 0;
}
