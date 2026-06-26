#include <array>
#include <iostream>
#include <string>
#include <utility>

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
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "END") break;
        std::cout << (is_balanced(line) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
