#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>

class StringBuffer {
    char* text;
    std::size_t len;

    void swapContents(StringBuffer& other)
    {
        std::swap(text, other.text);
        std::swap(len, other.len);
    }

public:
    StringBuffer(const char* s) : text(nullptr), len(0) {
        len = std::strlen(s);
        text = new char[len + 1];
        std::memcpy(text, s, len + 1);
    }

    ~StringBuffer() { delete[] text; }

    StringBuffer(const StringBuffer& o) : text(nullptr), len(0) {
        len = o.len;
        text = new char[len + 1];
        std::memcpy(text, o.text, len + 1);
    }

    StringBuffer& operator=(StringBuffer tmp) {
        swapContents(tmp);
        return *this;
    }

    void trim()
    {
        std::size_t start = 0;
        while (start < len && std::isspace(static_cast<unsigned char>(text[start])))
            start++;
        std::size_t end = len;
        while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1])))
            end--;
        len = end - start;
        std::memmove(text, text + start, len);
        text[len] = '\0';
    }

    const char* c_str() const { return text; }
    std::size_t size() const { return len; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(text, o.text) < 0;
    }
};

int main()
{
    std::vector<StringBuffer> names;
    for (std::string line; std::getline(std::cin, line);)
        names.push_back(StringBuffer(line.c_str()));

    for (std::size_t idx = 0; idx < names.size(); ++idx)
        names[idx].trim();

    names.erase(std::remove_if(names.begin(), names.end(),
                               [](const StringBuffer& n) { return n.size() == 0; }),
                names.end());
    std::sort(names.begin(), names.end());
    for (const auto& n : names)
        std::cout << n.c_str() << '\n';
    return 0;
}
