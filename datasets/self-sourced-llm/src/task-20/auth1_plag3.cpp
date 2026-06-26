#include <utility>
#include <array>
#include <iostream>
#include <string>
// TODO: refactor later

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> pool_;
    std::size_t top_ = 0;

public:
    bool push(const T& value) {
        if (top_ >= N) return false;
        pool_[top_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (top_ >= N) return false;
        pool_[top_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (top_ == 0) return false;
        out = pool_[--top_];
        return true;
    }

    bool peek(T& out) const {
        if (top_ == 0) return false;
        out = pool_[top_ - 1];
        return true;
    }

    std::size_t size() const { return top_; }
};

static bool matches(char open, char close) {
    return (open == '(' && close == ')') || (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> st;
    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{') {
            if (!st.push(ch)) return false;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            char open;
            if (!st.pop(open)) return false;
            if (!matches(open, ch)) return false;
        }
    }
    return st.size() == 0;
}

int main() {
    std::string buf;
    while (std::getline(std::cin, buf)) {
        if (buf == "END") break;
        std::cout << (is_balanced(buf) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
