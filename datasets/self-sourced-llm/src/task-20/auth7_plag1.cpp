#include <iostream>
#include <string>
#include <utility>
#include <array>

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> backing;
    std::size_t count{0};

public:
    bool push(const T& value) {
        if (count >= N) return false;
        backing[count++] = value;
        return true;
    }

    bool push(T&& value) {
        if (count >= N) return false;
        backing[count++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (count == 0) return false;
        out = backing[--count];
        return true;
    }

    bool peek(T& out) const {
        if (count == 0) return false;
        out = backing[count - 1];
        return true;
    }

    std::size_t size() const { return count; }
};

namespace balance {

bool closing_matches(char open, char close) {
    if (open == '(') return close == ')';
    if (open == '[') return close == ']';
    if (open == '{') return close == '}';
    return false;
}

}  // namespace balance

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> brackets;
    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = static_cast<char>(static_cast<unsigned char>(s[i]));
        if (c == '(' || c == '[' || c == '{') {
            if (!brackets.push(c)) return false;
        } else if (c == ')' || c == ']' || c == '}') {
            char top;
            if (!brackets.pop(top)) return false;
            if (!balance::closing_matches(top, c)) return false;
        }
    }
    return brackets.size() == 0;
}

int main() {
    for (std::string line; std::getline(std::cin, line);) {
        if (line == "END") break;
        std::cout << (is_balanced(line) ? "OK" : "FAIL") << std::endl;
    }
    return 0;
}
