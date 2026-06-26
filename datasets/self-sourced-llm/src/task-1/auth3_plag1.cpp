#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>
// cleaned up version

// Heap-backed string for name normalization
class StringBuffer {
    char* str_;
    std::size_t n_;

    static bool isSpace(char c) {
        return std::isspace(static_cast<unsigned char>(c)) != 0;
    }

public:
    StringBuffer(const char* src) : str_(nullptr), n_(0) {
        n_ = std::strlen(src);
        str_ = new char[n_ + 1];
        std::memcpy(str_, src, n_ + 1);
    }

    ~StringBuffer() { delete[] str_; }

    StringBuffer(const StringBuffer& that) {
        n_ = that.n_;
        str_ = new char[n_ + 1];
        std::memcpy(str_, that.str_, n_ + 1);
    }

    StringBuffer& operator=(const StringBuffer& that) {
        if (this != &that) {
            char* fresh = new char[that.n_ + 1];
            std::memcpy(fresh, that.str_, that.n_ + 1);
            delete[] str_;
            str_ = fresh;
            n_ = that.n_;
        }
        return *this;
    }

    void trim() {
        std::size_t i = 0, j = n_;
        while (i < j && isSpace(str_[i])) ++i;
        while (j > i && isSpace(str_[j - 1])) --j;
        std::size_t k = 0;
        for (; i < j; ++i, ++k) str_[k] = str_[i];
        str_[k] = '\0';
        n_ = k;
    }

    const char* c_str() const { return str_; }
    std::size_t size() const { return n_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(str_, rhs.str_) < 0;
    }
};

static bool isBlank(const StringBuffer& s) { return s.size() == 0; }

int main() {
    std::vector<StringBuffer> items;
    for (std::string row; std::getline(std::cin, row); )
        items.emplace_back(StringBuffer(row.c_str()));

    for (StringBuffer& elem : items) elem.trim();
    items.erase(std::remove_if(items.begin(), items.end(), isBlank), items.end());
    std::sort(items.begin(), items.end());

    for (const StringBuffer& elem : items)
        std::cout << elem.c_str() << std::endl;
    return 0;
}
