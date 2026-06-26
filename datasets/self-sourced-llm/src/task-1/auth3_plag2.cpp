#include <vector>
#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>
// adapted from classmate submission

// Heap-backed string for name normalization
class StringBuffer {
    char* txt_;
    std::size_t sz_;

    static bool isSpace(char c) {
        return std::isspace(static_cast<unsigned char>(c)) != 0;
    }

public:
    StringBuffer(const char* src) : txt_(nullptr), sz_(0) {
        sz_ = std::strlen(src);
        txt_ = new char[sz_ + 1];
        std::memcpy(txt_, src, sz_ + 1);
    }

    ~StringBuffer() { delete[] txt_; }

    StringBuffer(const StringBuffer& that) {
        sz_ = that.sz_;
        txt_ = new char[sz_ + 1];
        std::memcpy(txt_, that.txt_, sz_ + 1);
    }

    StringBuffer& operator=(const StringBuffer& that) {
        if (this != &that)
{
            char* fresh = new char[that.sz_ + 1];
            std::memcpy(fresh, that.txt_, that.sz_ + 1);
            delete[] txt_;
            txt_ = fresh;
            sz_ = that.sz_;
        }
        return *this;
    }

    void trim() {
        std::size_t i = 0, j = sz_;
        while (i < j && isSpace(txt_[i])) ++i;
        while (j > i && isSpace(txt_[j - 1])) --j;
        std::size_t k = 0;
        for (; i < j; ++i, ++k) txt_[k] = txt_[i];
        txt_[k] = '\0';
        sz_ = k;
    }

    const char* c_str() const { return txt_; }
    std::size_t size() const { return sz_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(txt_, rhs.txt_) < 0;
    }
};

static bool isBlank(const StringBuffer& s) { return s.size() == 0; }

int main()
{
    std::vector<StringBuffer> batch;
    for (std::string ln; std::getline(std::cin, ln); )
        batch.push_back(StringBuffer(ln.c_str()));

    for (StringBuffer& item : batch) item.trim();
    batch.erase(std::remove_if(batch.begin(), batch.end(), isBlank), batch.end());
    std::sort(batch.begin(), batch.end());

    for (const StringBuffer& item : batch)
        std::cout << item.c_str() << "\n";
    return 0;
}
