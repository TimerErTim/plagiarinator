#include <array>
#include <iostream>
#include <string>
#include <utility>
// cleaned up version

template <typename T, std::size_t N>
class FixedStack {
public:
    bool push(const T& value) {
        if (size_ >= N) return false;
        cells_[size_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (size_ >= N) return false;
        cells_[size_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (size_ == 0) return false;
        out = cells_[--size_];
        return true;
    }

    bool peek(T& out) const {
        if (size_ == 0) return false;
        out = cells_[size_ - 1];
        return true;
    }

    std::size_t size() const { return size_; }

private:
    std::array<T, N> cells_{};
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
    for (char ch : s) {
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
    std::string s;
    while (std::getline(std::cin, s)) {
        if (s == "END") break;
        std::cout << (is_balanced(s) ? "OK" : "FAIL") << std::endl;
    }
    return 0;
}
