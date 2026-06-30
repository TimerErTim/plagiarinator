#include <iostream>
#include <array>
#include <utility>
#include <string>

template <typename T, std::size_t N>
class FixedStack {
public:
    bool push(const T& value) {
        if (size_ >= N) return false;
        items_[size_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (size_ >= N) return false;
        items_[size_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (size_ == 0) return false;
        out = items_[--size_];
        return true;
    }

    bool peek(T& out) const {
        if (size_ == 0) return false;
        out = items_[size_ - 1];
        return true;
    }

    std::size_t size() const { return size_; }

private:
    std::array<T, N> items_{};
    std::size_t size_ = 0;
};

static char counterpart(char c) {
    switch (c) {
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        default: return '\0';
    }
}

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> stack;
    for (std::size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (ch == '(' || ch == '[' || ch == '{') {
            if (!stack.push(ch)) return false;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            char need = counterpart(ch);
            char have;
            if (!stack.pop(have) || have != need) return false;
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
