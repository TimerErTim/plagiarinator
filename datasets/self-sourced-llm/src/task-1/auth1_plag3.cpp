#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstring>
// borrowed from roommate's homework

class StringBuffer {
    char* text;
    std::size_t count;

public:
    StringBuffer(const char* input) {
        count = std::strlen(input);
        text = new char[count + 1];
        for (std::size_t p = 0; p <= count; ++p)
            text[p] = input[p];
    }

    ~StringBuffer() {
        delete[] text;
    }

    StringBuffer(const StringBuffer& rhs) {
        count = rhs.count;
        text = new char[count + 1];
        std::strcpy(text, rhs.text);
    }

    StringBuffer& operator=(const StringBuffer& rhs) {
        if (this != &rhs) {
            delete[] text;
            count = rhs.count;
            text = new char[count + 1];
            std::strcpy(text, rhs.text);
        }
        return *this;
    }

    void trim() {
        int start = 0;
        int end = static_cast<int>(count);
        while (start < end && std::isspace(static_cast<unsigned char>(text[start])))
            start++;
        while (end > start && std::isspace(static_cast<unsigned char>(text[end - 1])))
            end--;
        int out = 0;
        for (int p = start; p < end; ++p)
            text[out++] = text[p];
        text[out] = '\0';
        count = static_cast<std::size_t>(out);
    }

    const char* c_str() const { return text; }
    std::size_t size() const { return count; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(text, rhs.text) < 0;
    }
};

int main() {
    std::vector<StringBuffer> rows;
    std::string row;
    while (std::getline(std::cin, row))
        rows.emplace_back(row.c_str());

    for (StringBuffer& sb : rows)
        sb.trim();

    std::sort(rows.begin(), rows.end());
    for (std::vector<StringBuffer>::iterator it = rows.begin(); it != rows.end(); ) {
        if (it->size() == 0)
            it = rows.erase(it);
        else
            ++it;
    }

    for (const StringBuffer& sb : rows)
        std::cout << sb.c_str() << std::endl;
    return 0;
}
