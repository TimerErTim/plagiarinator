#include <array>
#include <utility>
#include <iostream>
#include <string>
// stack lab from thursday

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> pile_;
    std::size_t depth_ = 0;

public:
    bool push(const T& value) {
        if (depth_ >= N) return false;
        pile_[depth_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (depth_ >= N) return false;
        pile_[depth_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (depth_ == 0) return false;
        out = pile_[--depth_];
        return true;
    }

    bool peek(T& out) const {
        if (depth_ == 0) return false;
        out = pile_[depth_ - 1];
        return true;
    }

    std::size_t size() const { return depth_; }
};

static bool mates(char lhs, char rhs) {
    return (lhs == '(' && rhs == ')') || (lhs == '[' && rhs == ']') ||
           (lhs == '{' && rhs == '}');
}

bool is_balanced(const std::string& expr) {
    FixedStack<char, 128> tower;
    for (char sym : expr) {
        if (sym == '(' || sym == '[' || sym == '{') {
            if (!tower.push(sym)) return false;
        } else if (sym == ')' || sym == ']' || sym == '}') {
            char opener;
            if (!tower.pop(opener)) return false;
            if (!mates(opener, sym)) return false;
        }
    }
    return tower.size() == 0;
}

int main() {
    std::string chunk;
    while (std::getline(std::cin, chunk)) {
        if (chunk == "END") break;
        std::cout << (is_balanced(chunk) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
