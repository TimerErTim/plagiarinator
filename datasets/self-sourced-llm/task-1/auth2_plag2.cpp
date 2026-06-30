#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <vector>

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

    void release()
    {
        delete[] buf;
        buf = nullptr;
        length = 0;
    }

    void copyFrom(const char* src, size_t n)
    {
        buf = new char[n + 1];
        std::memcpy(buf, src, n);
        buf[n] = '\0';
        length = n;
    }

public:
    explicit StringBuffer(const char* text)
    {
        length = std::strlen(text);
        copyFrom(text, length);
    }

    ~StringBuffer() { release(); }

    StringBuffer(const StringBuffer& other)
    {
        copyFrom(other.buf, other.length);
    }

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
        while (lo < hi && std::isspace(static_cast<unsigned char>(buf[lo]))) lo++;
        while (hi > lo && std::isspace(static_cast<unsigned char>(buf[hi - 1]))) hi--;
        std::memmove(buf, buf + lo, hi - lo);
        length = hi - lo;
        buf[length] = '\0';
    }

    const char* c_str() const { return buf; }
    size_t size() const { return length; }
    bool empty() const { return length == 0; }

    bool operator<(const StringBuffer& rhs) const
    {
        return std::strcmp(buf, rhs.buf) < 0;
    }
};

int main()
{
    vector<StringBuffer> entries;
    string raw;
    while (getline(cin, raw))
        entries.emplace_back(raw.c_str());

    for (size_t i = 0; i < entries.size(); i++)
        entries[i].trim();

    entries.erase(remove_if(entries.begin(), entries.end(),
        [](const StringBuffer& s){ return s.empty(); }), entries.end());
    sort(entries.begin(), entries.end());

    for (const auto& e : entries)
        cout << e.c_str() << '\n';
    return 0;
}
