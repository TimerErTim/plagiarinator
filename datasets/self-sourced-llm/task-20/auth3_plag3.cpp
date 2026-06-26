#include <utility>
#include <array>
#include <iostream>
#include <string>
// TODO: refactor later

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> body_;
    std::size_t len_ = 0;

    bool full() const { return len_ >= N; }

public:
    bool push(const T& value) {
        if (full()) return false;
        body_[len_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (full()) return false;
        body_[len_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (len_ == 0) return false;
        out = body_[--len_];
        return true;
    }

    bool peek(T& out) const {
        if (len_ == 0) return false;
        out = body_[len_ - 1];
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
    std::string buf;
    while (std::getline(std::cin, buf)) {
        if (buf == "END") break;
        std::cout << (is_balanced(buf) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
