#include <iostream>
#include <string>
#include <array>
#include <utility>

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> buffer_;
    std::size_t head_ = 0;

public:
    bool push(const T& value) {
        if (head_ >= N) return false;
        buffer_[head_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (head_ >= N) return false;
        buffer_[head_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (head_ == 0) return false;
        out = buffer_[--head_];
        return true;
    }

    bool peek(T& out) const {
        if (head_ == 0) return false;
        out = buffer_[head_ - 1];
        return true;
    }

    std::size_t size() const { return head_; }
};

static bool bracket_ok(char open, char close) {
    return (open == '(' && close == ')') || (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> stack;
    for (std::size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (ch == '(' || ch == '[' || ch == '{') {
            if (!stack.push(ch)) return false;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            char open;
            if (!stack.pop(open)) return false;
            if (!bracket_ok(open, ch)) return false;
        }
    }
    return stack.size() == 0;
}

int main() {
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row == "END") break;
        std::cout << (is_balanced(row) ? "OK" : "FAIL") << std::endl;
    }
    return 0;
}
