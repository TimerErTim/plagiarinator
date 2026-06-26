#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>

class StringBuffer {
    char* data_;
    std::size_t len_;

    static std::size_t rawLength(const char* s) { return std::strlen(s); }

    void replaceStorage(const char* src, std::size_t n) {
        delete[] data_;
        len_ = n;
        data_ = new char[n + 1];
        std::memcpy(data_, src, n + 1);
    }

public:
    StringBuffer(const char* s) : data_(nullptr), len_(0) {
        replaceStorage(s, rawLength(s));
    }

    ~StringBuffer() { delete[] data_; }

    StringBuffer(const StringBuffer& o) {
        len_ = o.len_;
        data_ = new char[len_ + 1];
        std::memcpy(data_, o.data_, len_ + 1);
    }

    StringBuffer& operator=(const StringBuffer& o) {
        if (this == &o) return *this;
        char* next = new char[o.len_ + 1];
        std::memcpy(next, o.data_, o.len_ + 1);
        delete[] data_;
        data_ = next;
        len_ = o.len_;
        return *this;
    }

    void trim() {
        std::size_t left = 0, right = len_;
        while (left < right && std::isspace(static_cast<unsigned char>(data_[left])))
            ++left;
        while (right > left && std::isspace(static_cast<unsigned char>(data_[right - 1])))
            --right;
        std::memmove(data_, data_ + left, right - left);
        len_ = right - left;
        data_[len_] = '\0';
    }

    const char* c_str() const { return data_; }
    std::size_t size() const { return len_; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(data_, o.data_) < 0;
    }
};

int main() {
    std::vector<StringBuffer> names;
    for (std::string line; std::getline(std::cin, line);)
        names.emplace_back(line.c_str());

    for (auto& entry : names)
        entry.trim();

    names.erase(std::remove_if(names.begin(), names.end(),
                               [](const StringBuffer& s) { return s.size() == 0; }),
                names.end());

    std::sort(names.begin(), names.end());
    for (const auto& entry : names)
        std::cout << entry.c_str() << '\n';
    return 0;
}
