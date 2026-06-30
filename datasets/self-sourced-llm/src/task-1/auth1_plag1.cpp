#include <vector>
#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>

class StringBuffer {
    char* heap_;
    std::size_t nchars_;

public:
    StringBuffer(const char* s) {
        nchars_ = std::strlen(s);
        heap_ = new char[nchars_ + 1];
        std::strcpy(heap_, s);
    }

    ~StringBuffer() { delete[] heap_; }

    StringBuffer(const StringBuffer& o) {
        nchars_ = o.nchars_;
        heap_ = new char[nchars_ + 1];
        std::strcpy(heap_, o.heap_);
    }

    StringBuffer& operator=(const StringBuffer& o) {
        if (this != &o) {
            delete[] heap_;
            nchars_ = o.nchars_;
            heap_ = new char[nchars_ + 1];
            std::strcpy(heap_, o.heap_);
        }
        return *this;
    }

    void trim() {
        std::size_t lo = 0;
        while (lo < nchars_ && std::isspace(static_cast<unsigned char>(heap_[lo])))
            lo++;
        std::size_t hi = nchars_;
        while (hi > lo && std::isspace(static_cast<unsigned char>(heap_[hi - 1])))
            hi--;
        std::size_t trimmed = hi - lo;
        for (std::size_t j = 0; j < trimmed; j++)
            heap_[j] = heap_[lo + j];
        heap_[trimmed] = '\0';
        nchars_ = trimmed;
    }

    const char* c_str() const { return heap_; }
    std::size_t size() const { return nchars_; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(heap_, o.heap_) < 0;
    }
};

int main() {
    std::vector<StringBuffer> entries;
    std::string row;
    while (std::getline(std::cin, row))
        entries.emplace_back(row.c_str());

    for (auto& item : entries)
        item.trim();

    entries.erase(std::remove_if(entries.begin(), entries.end(),
                                 [](const StringBuffer& s) { return s.size() == 0; }),
                  entries.end());
    std::sort(entries.begin(), entries.end());

    for (const auto& item : entries)
        std::cout << item.c_str() << std::endl;
    return 0;
}
