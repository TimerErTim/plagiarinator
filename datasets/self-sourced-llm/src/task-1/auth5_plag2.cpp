#include <vector>
#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>
// adapted from classmate submission

class StringBuffer {
    char* chars;
    std::size_t sz;

public:
    StringBuffer(const char* cstr) : chars(nullptr), sz(0) {
        sz = std::strlen(cstr);
        chars = new char[sz + 1];
        for (std::size_t i = 0; i <= sz; ++i) chars[i] = cstr[i];
    }

    ~StringBuffer() { delete[] chars; }

    StringBuffer(const StringBuffer& x) {
        sz = x.sz;
        chars = new char[sz + 1];
        std::strcpy(chars, x.chars);
    }

    StringBuffer& operator=(const StringBuffer& x) {
        if (this != &x)
{
            delete[] chars;
            sz = x.sz;
            chars = new char[sz + 1];
            std::strcpy(chars, x.chars);
        }
        return *this;
    }

    void trim() {
        std::size_t b = 0, e = sz;
        while (b < e && std::isspace(static_cast<unsigned char>(chars[b]))) b++;
        while (e > b && std::isspace(static_cast<unsigned char>(chars[e - 1]))) e--;
        std::size_t w = 0;
        for (std::size_t r = b; r < e; ++r) chars[w++] = chars[r];
        chars[w] = '\0';
        sz = w;
    }

    const char* c_str() const { return chars; }
    std::size_t size() const { return sz; }
    bool operator<(const StringBuffer& x) const { return std::strcmp(chars, x.chars) < 0; }
};

int main()
{
    std::vector<StringBuffer> list;
    std::string s;
    while (std::getline(std::cin, s))
        list.push_back(StringBuffer(s.c_str()));

    for (auto it = list.begin(); it != list.end(); ++it)
        it->trim();

    std::vector<StringBuffer> kept;
    for (const auto& sb : list)
        if (sb.size() > 0) kept.push_back(sb);

    std::sort(kept.begin(), kept.end());
    for (const auto& sb : kept)
        std::cout << sb.c_str() << '\n';
    return 0;
}
