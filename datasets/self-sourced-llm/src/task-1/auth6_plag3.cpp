#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cctype>
// borrowed from roommate's homework

namespace {

class StringBuffer {
    char* heap;
    std::size_t heapSize;

public:
    explicit StringBuffer(const char* input) {
        heapSize = std::strlen(input);
        heap = new char[heapSize + 1];
        std::strcpy(heap, input);
    }

    ~StringBuffer() { delete[] heap; }

    StringBuffer(const StringBuffer& peer) {
        heapSize = peer.heapSize;
        heap = new char[heapSize + 1];
        std::strcpy(heap, peer.heap);
    }

    StringBuffer& operator=(const StringBuffer& peer) {
        if (this == &peer) return *this;
        delete[] heap;
        heapSize = peer.heapSize;
        heap = new char[heapSize + 1];
        std::strcpy(heap, peer.heap);
        return *this;
    }

    void trim() {
        std::size_t lo = 0;
        while (lo < heapSize && std::isspace(static_cast<unsigned char>(heap[lo])))
            lo++;
        std::size_t hi = heapSize;
        while (hi > lo && std::isspace(static_cast<unsigned char>(heap[hi - 1])))
            hi--;
        std::size_t span = hi - lo;
        for (std::size_t k = span; k-- > 0;)
            heap[k] = heap[lo + k];
        heap[span] = '\0';
        heapSize = span;
    }

    const char* c_str() const { return heap; }
    std::size_t size() const { return heapSize; }

    bool operator<(const StringBuffer& peer) const {
        return std::strcmp(heap, peer.heap) < 0;
    }
};

}

int main() {
    std::vector<StringBuffer> lines;
    std::string lineBuf;
    while (std::getline(std::cin, lineBuf))
        lines.emplace_back(lineBuf.c_str());

    for (auto& ln : lines) ln.trim();
    lines.erase(std::remove_if(lines.begin(), lines.end(),
                              [](const StringBuffer& r) { return r.size() == 0; }),
               lines.end());
    std::sort(lines.begin(), lines.end());
    for (const StringBuffer& ln : lines)
        std::cout << ln.c_str() << std::endl;
    return 0;
}
