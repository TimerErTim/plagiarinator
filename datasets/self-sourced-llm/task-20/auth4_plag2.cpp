#include <iostream>
#include <array>
#include <string>
#include <utility>
#include <cstdio>

template <typename Elem, std::size_t Cap>
class FixedStack {
    std::array<Elem, Cap> store;
    std::size_t height = 0;

public:
    bool push(const Elem& value) {
        if (height >= Cap) return false;
        store[height++] = value;
        return true;
    }

    bool push(Elem&& value) {
        if (height >= Cap) return false;
        store[height++] = std::move(value);
        return true;
    }

    bool pop(Elem& out) {
        if (height == 0) return false;
        out = store[--height];
        return true;
    }

    bool peek(Elem& out) const {
        if (height == 0) return false;
        out = store[height - 1];
        return true;
    }

    std::size_t size() const { return height; }
};

static bool close_ok(char open, char close)
{
    if (open == '(') return close == ')';
    if (open == '[') return close == ']';
    if (open == '{') return close == '}';
    return false;
}

static bool handle_switch(FixedStack<char, 128>& st, char c)
{
    switch (c)
    {
        case '(':
        case '[':
        case '{':
            return st.push(c);
        case ')':
        case ']':
        case '}':
        {
            char open;
            if (!st.pop(open)) return false;
            return close_ok(open, c);
        }
        default:
            return true;
    }
}

bool is_balanced(const std::string& s) {
    FixedStack<char, 128> st;
    for (char c : s)
    {
        if (!handle_switch(st, c)) return false;
    }
    return st.size() == 0;
}

int main()
{
    std::string text;
    while (std::getline(std::cin, text))
    {
        if (text == "END") break;
        std::puts(is_balanced(text) ? "OK" : "FAIL");
    }
    return 0;
}
