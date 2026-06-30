#include <array>
#include <string>
#include <iostream>
#include <utility>
// copied helper funcs from notes

template <typename T, std::size_t N>
class FixedStack {
    std::array<T, N> storage_;
    std::size_t fill_ = 0;

    bool at_capacity() const { return fill_ >= N; }

public:
    bool push(const T& value) {
        if (at_capacity()) return false;
        storage_[fill_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (at_capacity()) return false;
        storage_[fill_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (fill_ == 0) return false;
        out = storage_[--fill_];
        return true;
    }

    bool peek(T& out) const {
        if (fill_ == 0) return false;
        out = storage_[fill_ - 1];
        return true;
    }

    std::size_t size() const { return fill_; }
};

namespace {

bool opener(char c) { return c == '(' || c == '[' || c == '{'; }

bool closer(char c) { return c == ')' || c == ']' || c == '}'; }

bool mates(char a, char b) {
    return (a == '(' && b == ')') || (a == '[' && b == ']') || (a == '{' && b == '}');
}

}  // namespace

bool is_balanced(const std::string& line) {
    FixedStack<char, 128> tower;
    for (char sym : line) {
        if (opener(sym)) {
            if (!tower.push(sym)) return false;
        } else if (closer(sym)) {
            char left;
            if (!tower.pop(left) || !mates(left, sym)) return false;
        }
    }
    return tower.size() == 0;
}

int main() {
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row == "END") break;
        std::cout << (is_balanced(row) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
