#include <utility>
#include <string>
#include <iostream>
// adapted from classmate submission

class StringBuilder {
    std::string payload_;

public:
    StringBuilder() = default;
    StringBuilder(StringBuilder&& src) noexcept : payload_(std::move(src.payload_)) {}
    StringBuilder& operator=(StringBuilder&& src) noexcept {
        payload_ = std::move(src.payload_);
        src.payload_.clear();
        return *this;
    }
    StringBuilder(const StringBuilder&) = delete;
    StringBuilder& operator=(const StringBuilder&) = delete;

    StringBuilder& append(const std::string& s) {
        payload_.append(s);
        return *this;
    }

    StringBuilder& append(std::string&& s) {
        payload_.append(std::move(s));
        return *this;
    }

    StringBuilder& append_line(const std::string& s) {
        payload_.append(s);
        payload_.append(1, '\n');
        return *this;
    }

    const std::string& str() const { return payload_; }
    void clear() { payload_.clear(); }
};

int main()
{
    StringBuilder final_doc;
    std::string op;

    while (std::cin >> op) {
        if (op == "END") break;
        if (op != "SECTION") continue;

        std::string heading;
        std::getline(std::cin, heading);
        while (!heading.empty() && heading.front() == ' ') heading.erase(0, 1);

        std::string assembled;
        assembled += "# ";
        assembled += heading;
        assembled += "\n";

        for (std::string input; std::getline(std::cin, input);) {
            if (input.empty()) break;
            assembled += input;
            assembled += '\n';
        }
        assembled += '\n';

        StringBuilder tmp;
        tmp.append(std::move(assembled));
        final_doc.append(std::move(tmp.str()));
    }

    std::cout << final_doc.str();
    return 0;
}
