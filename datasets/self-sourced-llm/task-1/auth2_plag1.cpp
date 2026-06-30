#include <vector>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <iostream>

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
    char* storage;
    size_t byteCount;

    void release() { delete[] storage; storage = nullptr; byteCount = 0; }

    void copyFrom(const char* src, size_t n)
    {
        storage = new char[n + 1];
        memcpy(storage, src, n);
        storage[n] = '\0';
        byteCount = n;
    }

public:
    explicit StringBuffer(const char* text)
    {
        byteCount = strlen(text);
        copyFrom(text, byteCount);
    }

    ~StringBuffer() { release(); }

    StringBuffer(const StringBuffer& other)
    {
        copyFrom(other.storage, other.byteCount);
    }

    StringBuffer& operator=(const StringBuffer& rhs)
    {
        if (this == &rhs) return *this;
        release();
        copyFrom(rhs.storage, rhs.byteCount);
        return *this;
    }

    void trim()
    {
        size_t lo = 0, hi = byteCount;
        while (lo < hi && isspace(static_cast<unsigned char>(storage[lo]))) lo++;
        while (hi > lo && isspace(static_cast<unsigned char>(storage[hi - 1]))) hi--;
        memmove(storage, storage + lo, hi - lo);
        byteCount = hi - lo;
        storage[byteCount] = '\0';
    }

    const char* c_str() const { return storage; }
    size_t size() const { return byteCount; }
    bool empty() const { return byteCount == 0; }

    bool operator<(const StringBuffer& rhs) const
    {
        return strcmp(storage, rhs.storage) < 0;
    }
};

int main()
{
    vector<StringBuffer> entries;
    string raw;
    while (getline(cin, raw))
        entries.push_back(raw.c_str());

    for (auto& e : entries) e.trim();
    entries.erase(remove_if(entries.begin(), entries.end(),
        [](const StringBuffer& s){ return s.empty(); }), entries.end());
    sort(entries.begin(), entries.end());

    for (const auto& e : entries)
        cout << e.c_str() << endl;
    return 0;
}
