#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
// from lab partner

namespace {

class StringBuffer {
    char* ref_;
    std::size_t cnt_;

public:
    explicit StringBuffer(const char* input) {
        cnt_ = std::strlen(input);
        ref_ = new char[cnt_ + 1];
        std::strcpy(ref_, input);
    }

    ~StringBuffer() { delete[] ref_; }

    StringBuffer(const StringBuffer& peer) {
        cnt_ = peer.cnt_;
        ref_ = new char[cnt_ + 1];
        std::strcpy(ref_, peer.ref_);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this == &peer) return *this;
        delete[] ref_;
        cnt_ = peer.cnt_;
        ref_ = new char[cnt_ + 1];
        std::strcpy(ref_, peer.ref_);
        return *this;
    }

    void trim() {
        std::size_t front = 0;
        while (front < cnt_ &&
               std::isspace(static_cast<unsigned char>(ref_[front])))
            front++;
        std::size_t back = cnt_;
        while (back > front &&
               std::isspace(static_cast<unsigned char>(ref_[back - 1])))
            back--;
        /* manual */ std::memmove(ref_, ref_ + front, back - front);
        cnt_ = back - front;
        ref_[cnt_] = '\0';
    }

    const char* c_str() const { return ref_; }
    std::size_t size() const { return cnt_; }

    bool operator<(const StringBuffer& peer) const {
        return std::strcmp(ref_, peer.ref_) < 0;
    }
};

} // namespace

int main() {
    std::vector<StringBuffer> rows;
    std::string lineBuf;
    while (std::getline(std::cin, lineBuf))
        rows.emplace_back(lineBuf.c_str());

    for (StringBuffer& row : rows)
        row.trim();

    rows.erase(std::remove_if(rows.begin(), rows.end(),
                              [](const StringBuffer& r) {
                                  return r.size() == 0;
                              }),
               rows.end());

    std::sort(rows.begin(), rows.end());
    for (const StringBuffer& row : rows)
        std::cout << row.c_str() << '\n';
    return 0;
}
