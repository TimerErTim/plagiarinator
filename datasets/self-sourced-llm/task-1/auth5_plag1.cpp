#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>
// cleaned up version

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
        if (this != &x) {
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

int main() {
    std::vector<StringBuffer> list;
    std::string s;
    while (std::getline(std::cin, s))
        list.emplace_back(StringBuffer(s.c_str()));

    for (auto it = list.begin(); it != list.end(); ++it)
        it->trim();

    std::vector<StringBuffer> filtered;
    for (const auto& elem : list)
        if (elem.size() > 0) filtered.emplace_back(elem);

    std::sort(filtered.begin(), filtered.end());
    for (const auto& elem : filtered)
        std::cout << elem.c_str() << std::endl;
    return 0;
}
