#include <iostream>
#include <vector>
#include <cstring>
#include <cctype>
#include <algorithm>

class StringBuffer {
    char* buf_;
    std::size_t n_;

public:
    StringBuffer(const char* s) {
        n_ = std::strlen(s);
        buf_ = new char[n_ + 1];
        std::strcpy(buf_, s);
    }

    ~StringBuffer() { delete[] buf_; }

    StringBuffer(const StringBuffer& o) {
        n_ = o.n_;
        buf_ = new char[n_ + 1];
        std::strcpy(buf_, o.buf_);
    }

    StringBuffer& operator=(const StringBuffer& o) {
        if (this != &o) {
            delete[] buf_;
            n_ = o.n_;
            buf_ = new char[n_ + 1];
            std::strcpy(buf_, o.buf_);
        }
        return *this;
    }

    void trim() {
        std::size_t i = 0;
        while (i < n_ && std::isspace(static_cast<unsigned char>(buf_[i])))
            i++;
        std::size_t j = n_;
        while (j > i && std::isspace(static_cast<unsigned char>(buf_[j - 1])))
            j--;
        std::size_t k = 0;
        while (i < j)
            buf_[k++] = buf_[i++];
        buf_[k] = '\0';
        n_ = k;
    }

    const char* c_str() const { return buf_; }
    std::size_t size() const { return n_; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(buf_, o.buf_) < 0;
    }
};

int main() {
    std::vector<StringBuffer> names;
    std::string line;
    while (std::getline(std::cin, line))
        names.push_back(StringBuffer(line.c_str()));

    for (std::size_t idx = 0; idx < names.size(); ++idx)
        names[idx].trim();

    std::vector<StringBuffer> kept;
    for (const StringBuffer& s : names)
        if (s.size() != 0)
            kept.push_back(s);

    std::sort(kept.begin(), kept.end());
    for (const StringBuffer& s : kept)
        std::cout << s.c_str() << std::endl;
    return 0;
}
