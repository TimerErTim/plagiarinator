#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cctype>
// borrowed from roommate's homework

class StringBuffer {
    char* rawPtr;
    std::size_t rawLen;

    static bool ws(char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; }

public:
    StringBuffer(const char* src) : rawPtr(nullptr), rawLen(0) {
        rawLen = std::strlen(src);
        rawPtr = new char[rawLen + 1];
        std::memcpy(rawPtr, src, rawLen + 1);
    }

    ~StringBuffer() { delete[] rawPtr; }

    StringBuffer(const StringBuffer& peer) {
        rawLen = peer.rawLen;
        rawPtr = new char[rawLen + 1];
        std::memcpy(rawPtr, peer.rawPtr, rawLen + 1);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this != &peer) {
            char* tmp = new char[peer.rawLen + 1];
            std::memcpy(tmp, peer.rawPtr, peer.rawLen + 1);
            delete[] rawPtr;
            rawPtr = tmp;
            rawLen = peer.rawLen;
        }
        return *this;
    }

    void trim() {
        std::size_t lo = 0, hi = rawLen;
        while (lo < hi && ws(rawPtr[lo])) ++lo;
        while (hi > lo && ws(rawPtr[hi - 1])) --hi;
        std::size_t out = hi - lo;
        for (std::size_t k = out; k-- > 0;)
            rawPtr[k] = rawPtr[lo + k];
        rawPtr[out] = '\0';
        rawLen = out;
    }

    const char* c_str() const { return rawPtr; }
    std::size_t size() const { return rawLen; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(rawPtr, rhs.rawPtr) < 0;
    }
};

static bool isBlank(const StringBuffer& s) { return s.size() == 0; }

int main() {
    std::vector<StringBuffer> names;
    for (std::string ln; std::getline(std::cin, ln); )
        names.emplace_back(ln.c_str());

    for (auto& nm : names) nm.trim();
    names.erase(std::remove_if(names.begin(), names.end(), isBlank), names.end());
    std::sort(names.begin(), names.end());

    for (const StringBuffer& nm : names)
        std::cout << nm.c_str() << std::endl;
    return 0;
}
