#include <utility>
#include <string>
#include <iostream>
#include <array>
// adapted from classmate submission

template <typename T, std::size_t N>
class FixedStack {
public:
    bool push(const T& value) {
        if (size_ >= N) return false;
        elems_[size_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (size_ >= N) return false;
        elems_[size_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (size_ == 0) return false;
        out = elems_[--size_];
        return true;
    }

    bool peek(T& out) const {
        if (size_ == 0) return false;
        out = elems_[size_ - 1];
        return true;
    }

    std::size_t size() const { return size_; }

private:
    std::array<T, N> elems_{};
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

int main()
{
    std::string input;
    while (std::getline(std::cin, input)) {
        if (input == "END") break;
        std::cout << (is_balanced(input) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
