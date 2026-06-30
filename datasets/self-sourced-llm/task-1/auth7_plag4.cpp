#include <cstring>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <vector>

class StringBuffer {
    char* text;
    std::size_t len;

    void swapContents(StringBuffer& other) {
        std::swap(text, other.text);
        std::swap(len, other.len);
    }

public:
    StringBuffer(const char* s) : text(nullptr), len(0) {
        len = std::strlen(s);
        text = new char[len + 1];
        std::strcpy(text, s);
    }

    ~StringBuffer() { delete[] text; }

    StringBuffer(const StringBuffer& o) : text(nullptr), len(0) {
        len = o.len;
        text = new char[len + 1];
        std::strcpy(text, o.text);
    }

    StringBuffer& operator=(StringBuffer tmp) {
        swapContents(tmp);
        return *this;
    }

    void trim() {
        std::size_t start = 0;
        while (start < len && std::isspace(static_cast<unsigned char>(text[start])))
            start++;
        std::size_t end = len;
        while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1])))
            end--;
        len = end - start;
        for (std::size_t i = 0; i < len; ++i)
            text[i] = text[start + i];
        text[len] = '\0';
    }

    const char* c_str() const { return text; }
    std::size_t size() const { return len; }

    bool operator<(const StringBuffer& o) const {
        return std::strcmp(text, o.text) < 0;
    }
};

int main() {
    std::vector<StringBuffer> items;
    for (std::string line; std::getline(std::cin, line);)
        items.emplace_back(line.c_str());

    for (auto& item : items) item.trim();
    items.erase(std::remove_if(items.begin(), items.end(),
                               [](const StringBuffer& n) { return n.size() == 0; }),
                items.end());
    std::sort(items.begin(), items.end());
    for (const auto& item : items)
        std::cout << item.c_str() << '\n';
    return 0;
}
