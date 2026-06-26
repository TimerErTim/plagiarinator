#include <cstring>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <vector>

class StringBuffer {
    char* data_;
    std::size_t len_;

public:
    StringBuffer(const char* s) {
        len_ = std::strlen(s);
        data_ = new char[len_ + 1];
        std::strcpy(data_, s);
    }

    ~StringBuffer() { delete[] data_; }

    StringBuffer(const StringBuffer& o) {
        len_ = o.len_;
        data_ = new char[len_ + 1];
        std::strcpy(data_, o.data_);
    }

    StringBuffer& operator=(const StringBuffer& o) {
        if (this != &o) {
            delete[] data_;
            len_ = o.len_;
            data_ = new char[len_ + 1];
            std::strcpy(data_, o.data_);
        }
        return *this;
    }

    void trim() {
        std::size_t a = 0;
        while (a < len_ && std::isspace(static_cast<unsigned char>(data_[a]))) ++a;
        std::size_t b = len_;
        while (b > a && std::isspace(static_cast<unsigned char>(data_[b - 1]))) --b;
        std::size_t m = b - a;
        for (std::size_t i = 0; i < m; ++i) data_[i] = data_[a + i];
        data_[m] = '\0';
        len_ = m;
    }

    const char* c_str() const { return data_; }
    std::size_t size() const { return len_; }
    bool operator<(const StringBuffer& o) const { return std::strcmp(data_, o.data_) < 0; }
};

int main() {
    std::vector<StringBuffer> names;
    std::string line;
    while (std::getline(std::cin, line)) names.emplace_back(line.c_str());
    for (auto& n : names) n.trim();
    names.erase(std::remove_if(names.begin(), names.end(),
                               [](const StringBuffer& s) { return s.size() == 0; }),
                names.end());
    std::sort(names.begin(), names.end());
    for (const auto& n : names) std::cout << n.c_str() << '\n';
    return 0;
}
