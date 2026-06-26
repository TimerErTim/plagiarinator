#include <array>
#include <iostream>
#include <string>
#include <utility>

template <typename T, std::size_t N>
class FixedStack {
public:
    bool push(const T& value) {
        if (top_index_ >= N) return false;
        storage_[top_index_++] = value;
        return true;
    }

    bool push(T&& value) {
        if (top_index_ >= N) return false;
        storage_[top_index_++] = std::move(value);
        return true;
    }

    bool pop(T& out) {
        if (top_index_ == 0) return false;
        out = storage_[--top_index_];
        return true;
    }

    bool peek(T& out) const {
        if (top_index_ == 0) return false;
        out = storage_[top_index_ - 1];
        return true;
    }

    std::size_t size() const { return top_index_; }

private:
    std::array<T, N> storage_{};
    std::size_t top_index_ = 0;
};

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> pile;
    for (std::size_t i = 0; i < s.size(); ++i) {
        char ch = s[i];
        if (ch == '(' || ch == '[' || ch == '{') {
            if (!pile.push(ch)) return false;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            char prior;
            if (!pile.pop(prior)) return false;
            bool ok = (prior == '(' && ch == ')') || (prior == '[' && ch == ']') ||
                      (prior == '{' && ch == '}');
            if (!ok) return false;
        }
    }
    return pile.size() == 0;
}

int main() {
    std::string input;
    while (std::getline(std::cin, input)) {
        if (input == "END") break;
        std::cout << (is_balanced(input) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
