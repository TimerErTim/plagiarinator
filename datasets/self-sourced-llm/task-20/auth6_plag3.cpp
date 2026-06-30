#include <iostream>
#include <string>
#include <array>
#include <utility>
// lambda for pair check seemed easier

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> slots;
    std::size_t top = 0;

public:
    bool push(const T& v) {
        if (top == N) return false;
        slots[top++] = v;
        return true;
    }

    bool push(T&& v) {
        if (top == N) return false;
        slots[top++] = std::move(v);
        return true;
    }

    bool pop(T& out) {
        if (top == 0) return false;
        out = slots[--top];
        return true;
    }

    bool peek(T& out) const {
        if (top == 0) return false;
        out = slots[top - 1];
        return true;
    }

    std::size_t size() const { return top; }
};

bool is_balanced(const std::string& expr) {
    FixedStack<char, 128> pile;
    auto pairs_ok = [](char a, char b) {
        return (a == '(' && b == ')') || (a == '[' && b == ']') || (a == '{' && b == '}');
    };

    for (char sym : expr) {
        if (sym == '(' || sym == '[' || sym == '{') {
            if (!pile.push(sym)) return false;
        } else if (sym == ')' || sym == ']' || sym == '}') {
            char left;
            if (!pile.pop(left)) return false;
            if (!pairs_ok(left, sym)) return false;
        }
    }
    char junk;
    return !pile.peek(junk);
}

int main() {
    std::string s;
    while (std::getline(std::cin, s)) {
        if (s == "END") break;
        std::cout << (is_balanced(s) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
