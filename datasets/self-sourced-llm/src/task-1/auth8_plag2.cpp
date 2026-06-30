#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>

class StringBuffer {
    char* storage;
    std::size_t length;

    static void dupBuffer(char*& dst, std::size_t& dstLen, const char* src, std::size_t srcLen)
    {
        dstLen = srcLen;
        dst = new char[srcLen + 1];
        std::memcpy(dst, src, srcLen + 1);
    }

public:
    StringBuffer(const char* raw) : storage(nullptr), length(0) {
        dupBuffer(storage, length, raw, std::strlen(raw));
    }

    ~StringBuffer() {
        delete[] storage;
    }

    StringBuffer(const StringBuffer& orig) : storage(nullptr), length(0) {
        dupBuffer(storage, length, orig.storage, orig.length);
    }

    StringBuffer& operator=(const StringBuffer& orig) {
        if (this != &orig) {
            delete[] storage;
            dupBuffer(storage, length, orig.storage, orig.length);
        }
        return *this;
    }

    void trim()
    {
        std::size_t l = 0, r = length;
        while (l < r && std::isspace(static_cast<unsigned char>(storage[l]))) l++;
        while (r > l && std::isspace(static_cast<unsigned char>(storage[r - 1]))) r--;
        std::size_t n = r - l;
        std::memmove(storage, storage + l, n);
        length = n;
        storage[length] = '\0';
    }

    const char* c_str() const { return storage; }
    std::size_t size() const { return length; }

    bool operator<(const StringBuffer& other) const {
        return std::strcmp(storage, other.storage) < 0;
    }
};

int main()
{
    std::vector<StringBuffer> collection;
    std::stringstream input;
    input << std::cin.rdbuf();
    std::string row;
    while (std::getline(input, row))
        collection.push_back(StringBuffer(row.c_str()));

    for (std::size_t i = 0; i < collection.size(); ++i)
        collection[i].trim();

    collection.erase(
        std::remove_if(collection.begin(), collection.end(),
                       [](const StringBuffer& sb) { return sb.size() == 0; }),
        collection.end());

    std::sort(collection.begin(), collection.end());
    for (const StringBuffer& sb : collection)
        std::cout << sb.c_str() << '\n';
    return 0;
}
