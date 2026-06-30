#include <array>
#include <string>
#include <iostream>
#include <utility>
// index loop instead of range-for

template <typename T, std::size_t N>
class FixedStack {
public:
    bool push(const T& value) {
        if (head_ >= N) return false;
        data_[head_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (head_ >= N) return false;
        data_[head_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (head_ == 0) return false;
        out = data_[--head_];
        return true;
    }

    bool peek(T& out) const {
        if (head_ == 0) return false;
        out = data_[head_ - 1];
        return true;
    }

    std::size_t size() const { return head_; }

private:
    std::array<T, N> data_{};
    std::size_t head_ = 0;
};

bool is_balanced(const std::string& expr) {
    FixedStack<char, 128> tower;
    for (std::size_t pos = 0; pos < expr.size(); ++pos) {
        char ch = expr[pos];
        if (ch == '(' || ch == '[' || ch == '{') {
            if (!tower.push(ch)) return false;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            char prior;
            if (!tower.pop(prior)) return false;
            bool ok = (prior == '(' && ch == ')') || (prior == '[' && ch == ']') ||
                      (prior == '{' && ch == '}');
            if (!ok) return false;
        }
    }
    return tower.size() == 0;
}

int main() {
    std::string input;
    while (std::getline(std::cin, input)) {
        if (input == "END") break;
        std::cout << (is_balanced(input) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
