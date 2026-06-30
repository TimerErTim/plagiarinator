#include <cstring>
#include <cctype>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

class StringBuffer {
    char* storage;
    std::size_t length;

public:
    StringBuffer(const char* raw) {
        length = std::strlen(raw);
        storage = new char[length + 1];
        std::memcpy(storage, raw, length + 1);
    }

    ~StringBuffer() { delete[] storage; }

    StringBuffer(const StringBuffer& orig) {
        length = orig.length;
        storage = new char[length + 1];
        std::memcpy(storage, orig.storage, length + 1);
    }

    StringBuffer& operator=(const StringBuffer& orig) {
        if (this != &orig) {
            delete[] storage;
            length = orig.length;
            storage = new char[length + 1];
            std::memcpy(storage, orig.storage, length + 1);
        }
        return *this;
    }

    void trim() {
        std::size_t l = 0, r = length;
        while (l < r && std::isspace(static_cast<unsigned char>(storage[l]))) l++;
        while (r > l && std::isspace(static_cast<unsigned char>(storage[r - 1]))) r--;
        for (std::size_t i = 0; l < r; l++, i++)
            storage[i] = storage[l];
        length = r - l;
        storage[length] = '\0';
    }

    const char* c_str() const { return storage; }
    std::size_t size() const { return length; }

    bool operator<(const StringBuffer& other) const {
        return std::strcmp(storage, other.storage) < 0;
    }
};

int main() {
    std::vector<StringBuffer> batch;
    std::stringstream input;
    input << std::cin.rdbuf();
    std::string row;
    while (std::getline(input, row))
        batch.emplace_back(row.c_str());

    for (StringBuffer& sb : batch) sb.trim();
    batch.erase(
        std::remove_if(batch.begin(), batch.end(),
                       [](const StringBuffer& sb) { return sb.size() == 0; }),
        batch.end());
    std::sort(batch.begin(), batch.end());
    for (const StringBuffer& sb : batch)
        std::cout << sb.c_str() << '\n';
    return 0;
}
