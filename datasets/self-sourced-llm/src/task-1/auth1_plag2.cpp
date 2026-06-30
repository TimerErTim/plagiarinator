#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>

class StringBuffer {
    char* data_;
    std::size_t len_;

    static void copyChars(char* dst, const char* src, std::size_t n) {
        std::memcpy(dst, src, n + 1);
    }

public:
    StringBuffer(const char* s) {
        len_ = std::strlen(s);
        data_ = new char[len_ + 1];
        copyChars(data_, s, len_);
    }

    ~StringBuffer() { delete[] data_; }

    StringBuffer(const StringBuffer& o) {
        len_ = o.len_;
        data_ = new char[len_ + 1];
        copyChars(data_, o.data_, len_);
    }

    StringBuffer& operator=(const StringBuffer& o) {
        if (this != &o) {
            delete[] data_;
            len_ = o.len_;
            data_ = new char[len_ + 1];
            copyChars(data_, o.data_, len_);
        }
        return *this;
    }

    void trim() {
        std::size_t start = 0;
        while (start < len_ && std::isspace(static_cast<unsigned char>(data_[start])))
            ++start;
        std::size_t end = len_;
        while (end > start && std::isspace(static_cast<unsigned char>(data_[end - 1])))
            --end;
        std::size_t newLen = end - start;
        for (std::size_t i = 0; i < newLen; ++i)
            data_[i] = data_[start + i];
        data_[newLen] = '\0';
        len_ = newLen;
    }

    const char* c_str() const { return data_; }
    std::size_t size() const { return len_; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(data_, o.data_) < 0;
    }
};

int main()
{
    std::vector<StringBuffer> names;
    std::string line;
    while (std::getline(std::cin, line))
        names.emplace_back(line.c_str());

    for (std::size_t i = 0; i < names.size(); ++i)
        names[i].trim();

    names.erase(std::remove_if(names.begin(), names.end(),
                               [](const StringBuffer& s) { return s.size() == 0; }),
                names.end());
    std::sort(names.begin(), names.end());

    for (const StringBuffer& n : names)
        std::cout << n.c_str() << '\n';
    return 0;
}
