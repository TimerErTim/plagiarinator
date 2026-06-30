#include <utility>
#include <string>
#include <array>
#include <iostream>
// namespace wrapper from tutorial

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> mem;
    std::size_t n{0};

public:
    bool push(const T& value) {
        if (n >= N) return false;
        mem[n++] = value;
        return true;
    }

    bool push(T&& value) {
        if (n >= N) return false;
        mem[n++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (n == 0) return false;
        out = mem[--n];
        return true;
    }

    bool peek(T& out) const {
        if (n == 0) return false;
        out = mem[n - 1];
        return true;
    }

    std::size_t size() const { return n; }
};

namespace balance {

bool closing_matches(char open, char close) {
    if (open == '(') return close == ')';
    if (open == '[') return close == ']';
    if (open == '{') return close == '}';
    return false;
}

}  // namespace balance

bool is_balanced(const std::string& text) {
    FixedStack<char, 128> stk;
    for (unsigned char raw : text) {
        char c = static_cast<char>(raw);
        if (c == '(' || c == '[' || c == '{') {
            if (!stk.push(c)) return false;
        } else if (c == ')' || c == ']' || c == '}') {
            char top;
            if (!stk.pop(top)) return false;
            if (!balance::closing_matches(top, c)) return false;
        }
    }
    return stk.size() == 0;
}

int main() {
    for (std::string line; std::getline(std::cin, line);) {
        if (line == "END") break;
        std::cout << (is_balanced(line) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
