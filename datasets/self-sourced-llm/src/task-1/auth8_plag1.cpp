#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
// cleaned up version

class StringBuffer {
    char* storage;
    std::size_t length;

public:
    StringBuffer(const char* raw) {
        length = std::strlen(raw);
        storage = new char[length + 1];
        std::memcpy(storage, raw, length + 1);
    }

    ~StringBuffer() {
        delete[] storage;
    }

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
        for (std::size_t i = 0; l < r; ++l, ++i)
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
    std::vector<StringBuffer> items;
    std::stringstream stream;
    stream << std::cin.rdbuf();
    std::string row;
    while (std::getline(stream, row))
        items.emplace_back(row.c_str());

    for (StringBuffer& elem : items)
        elem.trim();

    items.erase(
        std::remove_if(items.begin(), items.end(),
                       [](const StringBuffer& elem) { return elem.size() == 0; }),
        items.end());

    std::sort(items.begin(), items.end());
    for (const StringBuffer& elem : items)
        std::cout << elem.c_str() << std::endl;
    return 0;
}
