#include <iostream>
#include <array>
#include <utility>
#include <string>

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

static bool brackets_match(char prior, char ch)
{
    return (prior == '(' && ch == ')') || (prior == '[' && ch == ']') ||
           (prior == '{' && ch == '}');
}

static bool process_index(FixedStack<char, 128>& pile, const std::string& s, std::size_t i)
{
    char ch = s[i];
    if (ch == '(' || ch == '[' || ch == '{')
    {
        return pile.push(ch);
    }
    if (ch == ')' || ch == ']' || ch == '}')
    {
        char prior;
        if (!pile.pop(prior)) return false;
        return brackets_match(prior, ch);
    }
    return true;
}

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> pile;
    for (std::size_t i = 0; i < s.size(); ++i)
    {
        if (!process_index(pile, s, i)) return false;
    }
    return pile.size() == 0;
}

int main()
{
    std::string input;
    while (std::getline(std::cin, input))
    {
        if (input == "END") break;
        std::cout << (is_balanced(input) ? "OK" : "FAIL") << '\n';
    }
    return 0;
}
