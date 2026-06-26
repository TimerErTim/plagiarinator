#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
// from lab partner

// Heap-backed string for name normalization
class StringBuffer {
    char* ref_;
    std::size_t cnt_;

    static bool isSpace(char c) {
        return std::isspace(static_cast<unsigned char>(c)) != 0;
    }

public:
    StringBuffer(const char* src) : ref_(nullptr), cnt_(0) {
        cnt_ = std::strlen(src);
        ref_ = new char[cnt_ + 1];
        std::memcpy(ref_, src, cnt_ + 1);
    }

    ~StringBuffer() { delete[] ref_; }

    StringBuffer(const StringBuffer& that) {
        cnt_ = that.cnt_;
        ref_ = new char[cnt_ + 1];
        std::memcpy(ref_, that.ref_, cnt_ + 1);
    }

    StringBuffer& operator=(const StringBuffer& that) {
        if (this != &that) {
            char* fresh = new char[that.cnt_ + 1];
            std::memcpy(fresh, that.ref_, that.cnt_ + 1);
            delete[] ref_;
            ref_ = fresh;
            cnt_ = that.cnt_;
        }
        return *this;
    }

    void trim() {
        std::size_t i = 0, j = cnt_;
        while (i < j && isSpace(ref_[i])) ++i;
        while (j > i && isSpace(ref_[j - 1])) --j;
        std::size_t k = 0;
        for (; i < j; ++i, ++k) ref_[k] = ref_[i];
        ref_[k] = '\0';
        cnt_ = k;
    }

    const char* c_str() const { return ref_; }
    std::size_t size() const { return cnt_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(ref_, rhs.ref_) < 0;
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
