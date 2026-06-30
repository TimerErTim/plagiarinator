#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cctype>
// borrowed from roommate's homework

class StringBuffer {
    char* heap;
    std::size_t heapLen;

public:
    StringBuffer(const char* raw) {
        heapLen = std::strlen(raw);
        heap = new char[heapLen + 1];
        std::memcpy(heap, raw, heapLen + 1);
    }

    ~StringBuffer() { delete[] heap; }

    StringBuffer(const StringBuffer& orig) {
        heapLen = orig.heapLen;
        heap = new char[heapLen + 1];
        std::memcpy(heap, orig.heap, heapLen + 1);
    }

    StringBuffer& operator=(const StringBuffer& orig) {
        if (this != &orig) {
            delete[] heap;
            heapLen = orig.heapLen;
            heap = new char[heapLen + 1];
            std::memcpy(heap, orig.heap, heapLen + 1);
        }
        return *this;
    }

    void trim() {
        std::size_t lo = 0, hi = heapLen;
        while (lo < hi && std::isspace(static_cast<unsigned char>(heap[lo]))) lo++;
        while (hi > lo && std::isspace(static_cast<unsigned char>(heap[hi - 1]))) hi--;
        std::size_t out = hi - lo;
        for (std::size_t k = out; k-- > 0;)
            heap[k] = heap[lo + k];
        heap[out] = '\0';
        heapLen = out;
    }

    const char* c_str() const { return heap; }
    std::size_t size() const { return heapLen; }

    bool operator<(const StringBuffer& other) const {
        return std::strcmp(heap, other.heap) < 0;
    }
};

int main() {
    std::vector<StringBuffer> names;
    std::stringstream pipe;
    pipe << std::cin.rdbuf();
    std::string row;
    while (std::getline(pipe, row))
        names.emplace_back(row.c_str());

    for (auto& nm : names) nm.trim();
    names.erase(
        std::remove_if(names.begin(), names.end(),
                       [](const StringBuffer& sb) { return sb.size() == 0; }),
        names.end());
    std::sort(names.begin(), names.end());
    for (const StringBuffer& nm : names)
        std::cout << nm.c_str() << std::endl;
    return 0;
}
