#include <utility>
#include <array>
#include <iostream>
#include <string>
// TODO: refactor later

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
    for (unsigned char raw : s) {
        char c = static_cast<char>(raw);
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
    for (std::string buf; std::getline(std::cin, buf);) {
        if (buf == "END") break;
        std::cout << (is_balanced(buf) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
