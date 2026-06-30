#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
// borrowed from roommate's homework

class StringBuffer {
    char* txt;
    std::size_t txtLen;

public:
    StringBuffer(const char* raw) {
        txtLen = std::strlen(raw);
        txt = new char[txtLen + 1];
        std::strcpy(txt, raw);
    }

    ~StringBuffer() { delete[] txt; }

    StringBuffer(const StringBuffer& peer) {
        txtLen = peer.txtLen;
        txt = new char[txtLen + 1];
        std::strcpy(txt, peer.txt);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this != &peer) {
            delete[] txt;
            txtLen = peer.txtLen;
            txt = new char[txtLen + 1];
            std::strcpy(txt, peer.txt);
        }
        return *this;
    }

    void trim() {
        std::size_t left = 0;
        while (left < txtLen && std::isspace(static_cast<unsigned char>(txt[left])))
            left++;
        std::size_t right = txtLen;
        while (right > left && std::isspace(static_cast<unsigned char>(txt[right - 1])))
            right--;
        std::size_t outLen = right - left;
        for (std::size_t k = outLen; k-- > 0;)
            txt[k] = txt[left + k];
        txt[outLen] = '\0';
        txtLen = outLen;
    }

    const char* c_str() const { return txt; }
    std::size_t size() const { return txtLen; }

    bool operator<(const StringBuffer& peer) const {
        return std::strcmp(txt, peer.txt) < 0;
    }
};

int main() {
    std::vector<StringBuffer> roster;
    std::string ln;
    while (std::getline(std::cin, ln))
        roster.emplace_back(ln.c_str());

    for (auto& entry : roster)
        entry.trim();

    roster.erase(std::remove_if(roster.begin(), roster.end(),
                                [](const StringBuffer& s) { return s.size() == 0; }),
                 roster.end());
    std::sort(roster.begin(), roster.end());

    for (const auto& entry : roster)
        std::cout << entry.c_str() << std::endl;
    return 0;
}
