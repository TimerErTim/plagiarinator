#include <cctype>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
// quick submit

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
    std::vector<StringBuffer> collection;
    std::stringstream input;
    input << std::cin.rdbuf();
    std::string row;
    while (std::getline(input, row))
        collection.emplace_back(row.c_str());

    for (StringBuffer& sb : collection)
        sb.trim();

    collection.erase(
        std::remove_if(collection.begin(), collection.end(),
                       [](const StringBuffer& sb) { return sb.size() == 0; }),
        collection.end());

    std::sort(collection.begin(), collection.end());
    for (const StringBuffer& sb : collection)
        std::cout << sb.c_str() << '\n';
    return 0;
}
