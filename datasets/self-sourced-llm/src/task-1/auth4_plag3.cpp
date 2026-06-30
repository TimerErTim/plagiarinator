#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cctype>
// borrowed from roommate's homework

class StringBuffer {
private:
    char* textBuf;
    std::size_t textLen;

    void cloneFrom(const char* s, std::size_t n) {
        textBuf = new char[n + 1];
        std::strncpy(textBuf, s, n);
        textBuf[n] = '\0';
        textLen = n;
    }

public:
    StringBuffer(const char* s) { cloneFrom(s, std::strlen(s)); }

    ~StringBuffer() { delete[] textBuf; }

    StringBuffer(const StringBuffer& other) { cloneFrom(other.textBuf, other.textLen); }

    StringBuffer& operator=(const StringBuffer& other) {
        if (&other != this) {
            delete[] textBuf;
            cloneFrom(other.textBuf, other.textLen);
        }
        return *this;
    }

    void trim() {
        int lo = 0;
        int hi = static_cast<int>(textLen) - 1;
        while (lo <= hi && std::isspace(textBuf[lo])) lo++;
        while (hi >= lo && std::isspace(textBuf[hi])) hi--;
        int span = hi - lo + 1;
        if (span < 0) span = 0;
        for (int k = span; k-- > 0;)
            textBuf[k] = textBuf[lo + k];
        textBuf[span] = '\0';
        textLen = static_cast<std::size_t>(span);
    }

    const char* c_str() const { return textBuf; }
    std::size_t size() const { return textLen; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(textBuf, rhs.textBuf) < 0;
    }
};

int main() {
    std::vector<StringBuffer> roster;
    char lineBuf[256];
    while (std::cin.getline(lineBuf, 256))
        roster.emplace_back(lineBuf);

    for (auto& entry : roster) entry.trim();

    roster.erase(
        std::remove_if(roster.begin(), roster.end(),
            [](const StringBuffer& sb) { return sb.size() == 0; }),
        roster.end());

    std::sort(roster.begin(), roster.end());

    for (const StringBuffer& entry : roster)
        std::cout << entry.c_str() << std::endl;
    return 0;
}
