#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>
#include <iostream>

class StringBuffer {
    char* payload_;
    std::size_t len_;

public:
    StringBuffer(const char* s) {
        len_ = std::strlen(s);
        payload_ = new char[len_ + 1];
        std::strcpy(payload_, s);
    }

    ~StringBuffer() { delete[] payload_; }

    StringBuffer(const StringBuffer& o) {
        len_ = o.len_;
        payload_ = new char[len_ + 1];
        std::strcpy(payload_, o.payload_);
    }

    StringBuffer& operator=(const StringBuffer& o) {
        if (this != &o) {
            delete[] payload_;
            len_ = o.len_;
            payload_ = new char[len_ + 1];
            std::strcpy(payload_, o.payload_);
        }
        return *this;
    }

    void trim() {
        std::size_t start = 0;
        while (start < len_ && std::isspace(static_cast<unsigned char>(payload_[start])))
            ++start;
        std::size_t end = len_;
        while (end > start && std::isspace(static_cast<unsigned char>(payload_[end - 1])))
            --end;
        std::size_t newLen = end - start;
        for (std::size_t i = 0; i < newLen; ++i)
            payload_[i] = payload_[start + i];
        payload_[newLen] = '\0';
        len_ = newLen;
    }

    const char* c_str() const { return payload_; }
    std::size_t size() const { return len_; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(payload_, o.payload_) < 0;
    }
};

int main() {
    std::vector<StringBuffer> batch;
    std::string raw;
    while (std::getline(std::cin, raw))
        batch.push_back(raw.c_str());

    for (auto& n : batch)
        n.trim();

    batch.erase(std::remove_if(batch.begin(), batch.end(),
                               [](const StringBuffer& s) { return s.size() == 0; }),
                batch.end());
    std::sort(batch.begin(), batch.end());

    for (const auto& n : batch)
        std::cout << n.c_str() << '\n';
    return 0;
}
