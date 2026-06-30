#include <utility>
#include <array>
#include <iostream>
#include <string>
// bracket checker v2

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> backing;
    std::size_t nItems = 0;

public:
    bool push(const T& value) {
        if (nItems == N) return false;
        backing[nItems++] = value;
        return true;
    }

    bool push(T&& value) {
        if (nItems == N) return false;
        backing[nItems++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (!nItems) return false;
        out = backing[--nItems];
        return true;
    }

    bool peek(T& out) const {
        if (!nItems) return false;
        out = backing[nItems - 1];
        return true;
    }

    std::size_t size() const { return nItems; }
};

static int brackets_match(char left, char right) {
    if (left == '(' && right == ')') return 1;
    if (left == '[' && right == ']') return 1;
    if (left == '{' && right == '}') return 1;
    return 0;
}

bool is_balanced(const std::string& text) {
    FixedStack<char, 128> pile;
    for (unsigned char raw : text) {
        char c = static_cast<char>(raw);
        if (c == '(' || c == '[' || c == '{') {
            if (!pile.push(c)) return false;
        } else if (c == ')' || c == ']' || c == '}') {
            char top;
            if (!pile.pop(top)) return false;
            if (!brackets_match(top, c)) return false;
        }
    }
    return pile.size() == 0;
}

int main() {
    for (std::string line; std::getline(std::cin, line);) {
        if (line == "END") break;
        std::cout << (is_balanced(line) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
