#include <array>
#include <iostream>
#include <utility>
#include <string>
// private members at bottom like slides

template <typename T, std::size_t N>
class FixedStack {
public:
    bool push(const T& value) {
        if (depth_ >= N) return false;
        bucket_[depth_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (depth_ >= N) return false;
        bucket_[depth_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (depth_ == 0) return false;
        out = bucket_[--depth_];
        return true;
    }

    bool peek(T& out) const {
        if (depth_ == 0) return false;
        out = bucket_[depth_ - 1];
        return true;
    }

    std::size_t size() const { return depth_; }

private:
    std::array<T, N> bucket_{};
    std::size_t depth_ = 0;
};

static char mirror(char c) {
    switch (c) {
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        default: return '\0';
    }
}

bool is_balanced(const std::string& line) {
    FixedStack<char, 128> tower;
    for (char sym : line) {
        if (sym == '(' || sym == '[' || sym == '{') {
            if (!tower.push(sym)) return false;
        } else if (sym == ')' || sym == ']' || sym == '}') {
            char need = mirror(sym);
            char got;
            if (!tower.pop(got) || got != need) return false;
        }
    }
    return tower.size() == 0;
}

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "END") break;
        std::cout << (is_balanced(line) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
