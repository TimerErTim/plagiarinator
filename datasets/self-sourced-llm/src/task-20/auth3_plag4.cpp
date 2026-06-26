#include <iostream>
#include <array>
#include <utility>
#include <string>
// quick submit

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> word_;
    std::size_t len_ = 0;

    bool full() const { return len_ >= N; }

public:
    bool push(const T& value) {
        if (full()) return false;
        word_[len_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (full()) return false;
        word_[len_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (len_ == 0) return false;
        out = word_[--len_];
        return true;
    }

    bool peek(T& out) const {
        if (len_ == 0) return false;
        out = word_[len_ - 1];
        return true;
    }

    std::size_t size() const { return len_; }
};

namespace {

bool is_open(char c) { return c == '(' || c == '[' || c == '{'; }

bool is_close(char c) { return c == ')' || c == ']' || c == '}'; }

bool pairs(char o, char c) {
    return (o == '(' && c == ')') || (o == '[' && c == ']') || (o == '{' && c == '}');
}

}  // namespace

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> stack;
    for (char ch : s) {
        if (is_open(ch)) {
            if (!stack.push(ch)) return false;
        } else if (is_close(ch)) {
            char prev;
            if (!stack.pop(prev) || !pairs(prev, ch)) return false;
        }
    }
    return stack.size() == 0;
}

int main() {
    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "END") break;
        std::cout << (is_balanced(str) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
