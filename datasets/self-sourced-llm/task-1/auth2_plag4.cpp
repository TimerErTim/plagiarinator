#include <cstring>
#include <cctype>
#include <iostream>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::getline;
using std::remove_if;
using std::sort;
using std::string;
using std::vector;

class StringBuffer
{
    char* buf;
    size_t length;

    void release() { delete[] buf; buf = nullptr; length = 0; }

    void copyFrom(const char* src, size_t n)
    {
        buf = new char[n + 1];
        memcpy(buf, src, n);
        buf[n] = '\0';
        length = n;
    }

public:
    explicit StringBuffer(const char* text)
    {
        length = strlen(text);
        copyFrom(text, length);
    }

    ~StringBuffer() { release(); }

    StringBuffer(const StringBuffer& other) { copyFrom(other.buf, other.length); }

    StringBuffer& operator=(const StringBuffer& rhs)
    {
        if (this == &rhs) return *this;
        release();
        copyFrom(rhs.buf, rhs.length);
        return *this;
    }

    void trim()
    {
        size_t lo = 0, hi = length;
        while (lo < hi && isspace(static_cast<unsigned char>(buf[lo]))) lo++;
        while (hi > lo && isspace(static_cast<unsigned char>(buf[hi - 1]))) hi--;
        memmove(buf, buf + lo, hi - lo);
        length = hi - lo;
        buf[length] = '\0';
    }

    const char* c_str() const { return buf; }
    size_t size() const { return length; }
    bool empty() const { return length == 0; }

    bool operator<(const StringBuffer& rhs) const { return strcmp(buf, rhs.buf) < 0; }
};

int main()
{
    vector<StringBuffer> roster;
    string raw;
    while (getline(cin, raw))
        roster.push_back(raw.c_str());

    for (auto& e : roster) e.trim();
    roster.erase(remove_if(roster.begin(), roster.end(),
        [](const StringBuffer& s){ return s.empty(); }), roster.end());
    sort(roster.begin(), roster.end());

    for (const auto& e : roster)
        cout << e.c_str() << '\n';
    return 0;
}
