#include <utility>
#include <iostream>
#include <array>
#include <string>
#include <cstdio>
// switch version works fine

template <typename Elem, std::size_t Cap>
class FixedStack {
    std::array<Elem, Cap> heap;
    std::size_t level = 0;

public:
    bool push(const Elem& value) {
        if (level >= Cap) return false;
        heap[level++] = value;
        return true;
    }

    bool push(Elem&& value) {
        if (level >= Cap) return false;
        heap[level++] = std::move(value);
        return true;
    }

    bool pop(Elem& out) {
        if (level == 0) return false;
        out = heap[--level];
        return true;
    }

    bool peek(Elem& out) const {
        if (level == 0) return false;
        out = heap[level - 1];
        return true;
    }

    std::size_t size() const { return level; }
};

bool is_balanced(const std::string& expr) {
    FixedStack<char, 128> pile;
    for (char sym : expr) {
        switch (sym) {
            case '(':
            case '[':
            case '{':
                if (!pile.push(sym)) return false;
                break;
            case ')':
            case ']':
            case '}': {
                char left;
                if (!pile.pop(left)) return false;
                if ((left == '(' && sym != ')') || (left == '[' && sym != ']') ||
                    (left == '{' && sym != '}'))
                    return false;
                break;
            }
            default:
                break;
        }
    }
    return pile.size() == 0;
}

int main() {
    std::string text;
    while (std::getline(std::cin, text)) {
        if (text == "END") break;
        std::puts(is_balanced(text) ? "OK" : "FAIL");
    }
    return 0;
}
