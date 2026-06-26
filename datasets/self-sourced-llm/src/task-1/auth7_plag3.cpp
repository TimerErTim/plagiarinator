#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
// from lab partner

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
    std::vector<StringBuffer> buf;
    for (std::string str; std::getline(std::cin, str);)
        buf.emplace_back(str.c_str());

    for (auto& n : buf) n.trim();
    buf.erase(std::remove_if(buf.begin(), buf.end(),
                               [](const StringBuffer& n) { return n.size() == 0; }),
                buf.end());
    std::sort(buf.begin(), buf.end());
    for (const auto& n : buf)
        std::cout << n.c_str() << '\n';
    return 0;
}
