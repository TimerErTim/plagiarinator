#include <vector>
#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>
// adapted from classmate submission

namespace {

class StringBuffer {
    char* buf_;
    std::size_t sz_;

public:
    explicit StringBuffer(const char* input) {
        sz_ = std::strlen(input);
        buf_ = new char[sz_ + 1];
        std::strcpy(buf_, input);
    }

    ~StringBuffer() { delete[] buf_; }

    StringBuffer(const StringBuffer& peer) {
        sz_ = peer.sz_;
        buf_ = new char[sz_ + 1];
        std::strcpy(buf_, peer.buf_);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this == &peer) return *this;
        delete[] buf_;
        sz_ = peer.sz_;
        buf_ = new char[sz_ + 1];
        std::strcpy(buf_, peer.buf_);
        return *this;
    }

    void trim() {
        std::size_t front = 0;
        while (front < sz_ &&
               std::isspace(static_cast<unsigned char>(buf_[front])))
            front++;
        std::size_t back = sz_;
        while (back > front &&
               std::isspace(static_cast<unsigned char>(buf_[back - 1])))
            back--;
        std::memmove(buf_, buf_ + front, back - front);
        sz_ = back - front;
        buf_[sz_] = '\0';
    }

    const char* c_str() const { return buf_; }
    std::size_t size() const { return sz_; }

    bool operator<(const StringBuffer& peer) const {
        return std::strcmp(buf_, peer.buf_) < 0;
    }
};

} // namespace

int main()
{
    std::vector<StringBuffer> data;
    std::string lineBuf;
    while (std::getline(std::cin, lineBuf))
        data.emplace_back(lineBuf.c_str());

    for (StringBuffer& row : data)
        row.trim();

    data.erase(std::remove_if(data.begin(), data.end(),
                              [](const StringBuffer& r) {
                                  return r.size() == 0;
                              }),
               data.end());

    std::sort(data.begin(), data.end());
    for (const StringBuffer& row : data)
        std::cout << row.c_str() << '\n';
    return 0;
}
