#include <vector>
#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>

class StringBuffer {
    char* content_;
    std::size_t count_;

    static bool isSpace(char c) {
        return std::isspace(static_cast<unsigned char>(c)) != 0;
    }

public:
    StringBuffer(const char* src) : content_(nullptr), count_(0) {
        count_ = std::strlen(src);
        content_ = new char[count_ + 1];
        std::memcpy(content_, src, count_ + 1);
    }

    ~StringBuffer() { delete[] content_; }

    StringBuffer(const StringBuffer& that) {
        count_ = that.count_;
        content_ = new char[count_ + 1];
        std::memcpy(content_, that.content_, count_ + 1);
    }

    StringBuffer& operator=(const StringBuffer& that) {
        if (this != &that) {
            char* fresh = new char[that.count_ + 1];
            std::memcpy(fresh, that.content_, that.count_ + 1);
            delete[] content_;
            content_ = fresh;
            count_ = that.count_;
        }
        return *this;
    }

    void trim() {
        std::size_t i = 0, j = count_;
        while (i < j && isSpace(content_[i])) i++;
        while (j > i && isSpace(content_[j - 1])) j--;
        std::size_t k = 0;
        for (; i < j; i++, k++) content_[k] = content_[i];
        content_[k] = '\0';
        count_ = k;
    }

    const char* c_str() const { return content_; }
    std::size_t size() const { return count_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(content_, rhs.content_) < 0;
    }
};

static bool isBlank(const StringBuffer& s) { return s.size() == 0; }

int main() {
    std::vector<StringBuffer> batch;
    for (std::string ln; std::getline(std::cin, ln); )
        batch.emplace_back(ln.c_str());

    for (StringBuffer& item : batch) item.trim();
    batch.erase(std::remove_if(batch.begin(), batch.end(), isBlank), batch.end());
    std::sort(batch.begin(), batch.end());

    for (const StringBuffer& item : batch)
        std::cout << item.c_str() << std::endl;
    return 0;
}
