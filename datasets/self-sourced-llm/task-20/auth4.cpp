#include <array>
#include <iostream>
#include <string>
#include <utility>

template <typename Elem, std::size_t Cap>
class FixedStack {
    std::array<Elem, Cap> store;
    std::size_t height = 0;

public:
    bool push(const Elem& value) {
        if (height >= Cap) return false;
        store[height++] = value;
        return true;
    }

    bool push(Elem&& value) {
        if (height >= Cap) return false;
        store[height++] = std::move(value);
        return true;
    }

    bool pop(Elem& out) {
        if (height == 0) return false;
        out = store[--height];
        return true;
    }

    bool peek(Elem& out) const {
        if (height == 0) return false;
        out = store[height - 1];
        return true;
    }

    std::size_t size() const { return height; }
};

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> st;
    for (char c : s) {
        switch (c) {
            case '(':
            case '[':
            case '{':
                if (!st.push(c)) return false;
                break;
            case ')':
            case ']':
            case '}': {
                char open;
                if (!st.pop(open)) return false;
                if ((open == '(' && c != ')') || (open == '[' && c != ']') ||
                    (open == '{' && c != '}'))
                    return false;
                break;
            }
            default:
                break;
        }
    }
    return st.size() == 0;
}

int main() {
    std::string text;
    while (std::getline(std::cin, text)) {
        if (text == "END") break;
        std::puts(is_balanced(text) ? "OK" : "FAIL");
    }
    return 0;
}

#include <cstdio>
