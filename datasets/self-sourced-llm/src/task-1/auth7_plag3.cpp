#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cctype>
// borrowed from roommate's homework

class StringBuffer {
    char* bytes;
    std::size_t byteCount;

    void swapWith(StringBuffer& other) {
        std::swap(bytes, other.bytes);
        std::swap(byteCount, other.byteCount);
    }

public:
    StringBuffer(const char* s) : bytes(nullptr), byteCount(0) {
        byteCount = std::strlen(s);
        bytes = new char[byteCount + 1];
        std::strcpy(bytes, s);
    }

    ~StringBuffer() { delete[] bytes; }

    StringBuffer(const StringBuffer& o) : bytes(nullptr), byteCount(0) {
        byteCount = o.byteCount;
        bytes = new char[byteCount + 1];
        std::strcpy(bytes, o.bytes);
    }

    StringBuffer& operator=(StringBuffer tmp) {
        swapWith(tmp);
        return *this;
    }

    void trim() {
        std::size_t lo = 0;
        while (lo < byteCount && std::isspace(static_cast<unsigned char>(bytes[lo])))
            lo++;
        std::size_t hi = byteCount;
        while (hi > lo && std::isspace(static_cast<unsigned char>(bytes[hi - 1])))
            hi--;
        std::size_t n = hi - lo;
        for (std::size_t k = n; k-- > 0;)
            bytes[k] = bytes[lo + k];
        bytes[n] = '\0';
        byteCount = n;
    }

    const char* c_str() const { return bytes; }
    std::size_t size() const { return byteCount; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(bytes, o.bytes) < 0;
    }
};

int main() {
    std::vector<StringBuffer> roster;
    for (std::string line; std::getline(std::cin, line);)
        roster.emplace_back(line.c_str());

    for (auto& row : roster) row.trim();
    roster.erase(std::remove_if(roster.begin(), roster.end(),
                               [](const StringBuffer& n) { return n.size() == 0; }),
                roster.end());
    std::sort(roster.begin(), roster.end());
    for (const auto& row : roster)
        std::cout << row.c_str() << std::endl;
    return 0;
}
