#include <string>
#include <iostream>
#include <utility>
#include <array>

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> data;
    std::size_t count = 0;

public:
    bool push(const T& value) {
        if (count == N) return false;
        data[count++] = value;
        return true;
    }

    bool push(T&& value) {
        if (count == N) return false;
        data[count++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (!count) return false;
        out = data[--count];
        return true;
    }

    bool peek(T& out) const {
        if (!count) return false;
        out = data[count - 1];
        return true;
    }

    std::size_t size() const { return count; }
};

static int pair_score(char a, char b) {
    if (a == '(' && b == ')') return 1;
    if (a == '[' && b == ']') return 1;
    if (a == '{' && b == '}') return 1;
    return 0;
}

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> stk;
    for (std::size_t i = 0; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c == '(' || c == '[' || c == '{') {
            if (!stk.push(static_cast<char>(c))) return false;
        } else if (c == ')' || c == ']' || c == '}') {
            char top;
            if (!stk.pop(top)) return false;
            if (!pair_score(top, static_cast<char>(c))) return false;
        }
    }
    return stk.size() == 0;
}

int main() {
    std::string input;
    for (; std::getline(std::cin, input);) {
        if (input == "END") break;
        std::cout << (is_balanced(input) ? "OK" : "FAIL") << std::endl;
    }
    return 0;
}
