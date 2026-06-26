#include <cctype>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
// quick submit

// Heap-backed string for name normalization
class StringBuffer {
    char* word_;
    std::size_t tmp_;

    static bool isSpace(char c) {
        return std::isspace(static_cast<unsigned char>(c)) != 0;
    }

public:
    StringBuffer(const char* src) : word_(nullptr), tmp_(0) {
        tmp_ = std::strlen(src);
        word_ = new char[tmp_ + 1];
        std::memcpy(word_, src, tmp_ + 1);
    }

    ~StringBuffer() { delete[] word_; }

    StringBuffer(const StringBuffer& that) {
        tmp_ = that.tmp_;
        word_ = new char[tmp_ + 1];
        std::memcpy(word_, that.word_, tmp_ + 1);
    }

    StringBuffer& operator=(const StringBuffer& that) {
        if (this != &that) {
            char* fresh = new char[that.tmp_ + 1];
            std::memcpy(fresh, that.word_, that.tmp_ + 1);
            delete[] word_;
            word_ = fresh;
            tmp_ = that.tmp_;
        }
        return *this;
    }

    void trim() {
        std::size_t i = 0, j = tmp_;
        while (i < j && isSpace(word_[i])) ++i;
        while (j > i && isSpace(word_[j - 1])) --j;
        std::size_t k = 0;
        for (; i < j; ++i, ++k) word_[k] = word_[i];
        word_[k] = '\0';
        tmp_ = k;
    }

    const char* c_str() const { return word_; }
    std::size_t size() const { return tmp_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(word_, rhs.word_) < 0;
    }
};

static bool isBlank(const StringBuffer& s) { return s.size() == 0; }

int main() {
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
