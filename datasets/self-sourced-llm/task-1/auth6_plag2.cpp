#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>

namespace {

class StringBuffer {
    char* payload_;
    std::size_t count_;

    void assignCopy(const char* src, std::size_t n)
    {
        payload_ = new char[n + 1];
        std::memcpy(payload_, src, n);
        payload_[n] = '\0';
        count_ = n;
    }

public:
    explicit StringBuffer(const char* input) {
        assignCopy(input, std::strlen(input));
    }

    ~StringBuffer() { delete[] payload_; }

    StringBuffer(const StringBuffer& peer) {
        assignCopy(peer.payload_, peer.count_);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this == &peer) return *this;
        delete[] payload_;
        assignCopy(peer.payload_, peer.count_);
        return *this;
    }

    void trim()
    {
        std::size_t front = 0;
        while (front < count_ &&
               std::isspace(static_cast<unsigned char>(payload_[front])))
            front++;
        std::size_t back = count_;
        while (back > front &&
               std::isspace(static_cast<unsigned char>(payload_[back - 1])))
            back--;
        std::memmove(payload_, payload_ + front, back - front);
        count_ = back - front;
        payload_[count_] = '\0';
    }

    const char* c_str() const { return payload_; }
    std::size_t size() const { return count_; }

    bool operator<(const StringBuffer& peer) const {
        return std::strcmp(payload_, peer.payload_) < 0;
    }
};

}

int main()
{
    std::vector<StringBuffer> rows;
    std::string lineBuf;
    while (std::getline(std::cin, lineBuf))
        rows.emplace_back(lineBuf.c_str());

    for (std::size_t i = 0; i < rows.size(); ++i)
        rows[i].trim();

    rows.erase(std::remove_if(rows.begin(), rows.end(),
                              [](const StringBuffer& r) { return r.size() == 0; }),
               rows.end());

    std::sort(rows.begin(), rows.end());
    for (const StringBuffer& row : rows)
        std::cout << row.c_str() << '\n';
    return 0;
}
