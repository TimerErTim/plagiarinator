#include <cstring>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <vector>

namespace {

class StringBuffer {
    char* payload_;
    std::size_t count_;

public:
    explicit StringBuffer(const char* input) {
        count_ = std::strlen(input);
        payload_ = new char[count_ + 1];
        std::strcpy(payload_, input);
    }

    ~StringBuffer() { delete[] payload_; }

    StringBuffer(const StringBuffer& peer) {
        count_ = peer.count_;
        payload_ = new char[count_ + 1];
        std::strcpy(payload_, peer.payload_);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this == &peer) return *this;
        delete[] payload_;
        count_ = peer.count_;
        payload_ = new char[count_ + 1];
        std::strcpy(payload_, peer.payload_);
        return *this;
    }

    void trim() {
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

int main() {
    std::vector<StringBuffer> batch;
    std::string lineBuf;
    while (std::getline(std::cin, lineBuf))
        batch.emplace_back(lineBuf.c_str());

    for (StringBuffer& row : batch) row.trim();
    batch.erase(std::remove_if(batch.begin(), batch.end(),
                              [](const StringBuffer& r) { return r.size() == 0; }),
               batch.end());
    std::sort(batch.begin(), batch.end());
    for (const StringBuffer& row : batch)
        std::cout << row.c_str() << '\n';
    return 0;
}
