#include <vector>
#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>
// borrowed from roommate's homework

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::remove_if;
using std::sort;
using std::string;
using std::vector;

class StringBuffer
{
    char* chars;
    size_t charLen;

    void drop() { delete[] chars; chars = nullptr; charLen = 0; }

    void dupFrom(const char* src, size_t n)
    {
        chars = new char[n + 1];
        memcpy(chars, src, n);
        chars[n] = '\0';
        charLen = n;
    }

public:
    explicit StringBuffer(const char* text)
    {
        charLen = strlen(text);
        dupFrom(text, charLen);
    }

    ~StringBuffer() { drop(); }

    StringBuffer(const StringBuffer& other) { dupFrom(other.chars, other.charLen); }

    StringBuffer& operator=(const StringBuffer& rhs)
    {
        if (this == &rhs) return *this;
        drop();
        dupFrom(rhs.chars, rhs.charLen);
        return *this;
    }

    void trim()
    {
        size_t lo = 0, hi = charLen;
        while (lo < hi && isspace(static_cast<unsigned char>(chars[lo]))) lo++;
        while (hi > lo && isspace(static_cast<unsigned char>(chars[hi - 1]))) hi--;
        memmove(chars, chars + lo, hi - lo);
        charLen = hi - lo;
        chars[charLen] = '\0';
    }

    const char* c_str() const { return chars; }
    size_t size() const { return charLen; }
    bool empty() const { return charLen == 0; }

    bool operator<(const StringBuffer& rhs) const { return strcmp(chars, rhs.chars) < 0; }
};

int main()
{
    vector<StringBuffer> lines;
    string raw;
    while (getline(cin, raw))
        lines.emplace_back(raw.c_str());

    for (auto& row : lines) row.trim();
    lines.erase(remove_if(lines.begin(), lines.end(),
        [](const StringBuffer& s){ return s.empty(); }), lines.end());
    sort(lines.begin(), lines.end());

    for (const auto& row : lines)
        cout << row.c_str() << endl;
    return 0;
}
