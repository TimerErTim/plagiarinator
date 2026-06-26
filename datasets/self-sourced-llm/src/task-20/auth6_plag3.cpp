#include <utility>
#include <array>
#include <iostream>
#include <string>
// TODO: refactor later

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> cells;
    std::size_t sp = 0;

public:
    bool push(const T& v) {
        if (sp == N) return false;
        cells[sp++] = v;
        return true;
    }

    bool push(T&& v) {
        if (sp == N) return false;
        cells[sp++] = std::move(v);
        return true;
    }

    bool pop(T& out) {
        if (sp == 0) return false;
        out = cells[--sp];
        return true;
    }

    bool peek(T& out) const {
        if (sp == 0) return false;
        out = cells[sp - 1];
        return true;
    }

    std::size_t size() const { return sp; }
};

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> st;
    auto good_pair = [](char a, char b) {
        return (a == '(' && b == ')') || (a == '[' && b == ']') || (a == '{' && b == '}');
    };

    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            if (!st.push(c)) return false;
        } else if (c == ')' || c == ']' || c == '}') {
            char open;
            if (!st.pop(open)) return false;
            if (!good_pair(open, c)) return false;
        }
    }
    char dummy;
    return !st.peek(dummy);
}

int main() {
    std::string s;
    while (std::getline(std::cin, s)) {
        if (s == "END") break;
        std::cout << (is_balanced(s) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
