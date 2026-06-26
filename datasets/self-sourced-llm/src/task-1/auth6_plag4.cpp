#include <cctype>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
// quick submit

namespace {

class StringBuffer {
    char* word_;
    std::size_t tmp_;

public:
    explicit StringBuffer(const char* input) {
        tmp_ = std::strlen(input);
        word_ = new char[tmp_ + 1];
        std::strcpy(word_, input);
    }

    ~StringBuffer() { delete[] word_; }

    StringBuffer(const StringBuffer& peer) {
        tmp_ = peer.tmp_;
        word_ = new char[tmp_ + 1];
        std::strcpy(word_, peer.word_);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this == &peer) return *this;
        delete[] word_;
        tmp_ = peer.tmp_;
        word_ = new char[tmp_ + 1];
        std::strcpy(word_, peer.word_);
        return *this;
    }

    void trim() {
        std::size_t front = 0;
        while (front < tmp_ &&
               std::isspace(static_cast<unsigned char>(word_[front])))
            front++;
        std::size_t back = tmp_;
        while (back > front &&
               std::isspace(static_cast<unsigned char>(word_[back - 1])))
            back--;
        std::memmove(word_, word_ + front, back - front);
        tmp_ = back - front;
        word_[tmp_] = '\0';
    }

    const char* c_str() const { return word_; }
    std::size_t size() const { return tmp_; }

    bool operator<(const StringBuffer& peer) const {
        return std::strcmp(word_, peer.word_) < 0;
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
