#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cctype>
// borrowed from roommate's homework

class StringBuffer {
    char* body;
    std::size_t bodyLen;

public:
    StringBuffer(const char* cstr) : body(nullptr), bodyLen(0) {
        bodyLen = std::strlen(cstr);
        body = new char[bodyLen + 1];
        std::strcpy(body, cstr);
    }

    ~StringBuffer() { delete[] body; }

    StringBuffer(const StringBuffer& x) {
        bodyLen = x.bodyLen;
        body = new char[bodyLen + 1];
        std::strcpy(body, x.body);
    }

    StringBuffer& operator=(const StringBuffer& x) {
        if (this != &x) {
            delete[] body;
            bodyLen = x.bodyLen;
            body = new char[bodyLen + 1];
            std::strcpy(body, x.body);
        }
        return *this;
    }

    void trim() {
        std::size_t b = 0, e = bodyLen;
        while (b < e && std::isspace(static_cast<unsigned char>(body[b]))) b++;
        while (e > b && std::isspace(static_cast<unsigned char>(body[e - 1]))) e--;
        std::size_t w = e - b;
        for (std::size_t k = w; k-- > 0;)
            body[k] = body[b + k];
        body[w] = '\0';
        bodyLen = w;
    }

    const char* c_str() const { return body; }
    std::size_t size() const { return bodyLen; }
    bool operator<(const StringBuffer& x) const { return std::strcmp(body, x.body) < 0; }
};

int main() {
    std::vector<StringBuffer> names;
    std::string s;
    while (std::getline(std::cin, s))
        names.emplace_back(s.c_str());

    for (auto& nm : names) nm.trim();

    std::vector<StringBuffer> kept;
    for (const auto& sb : names)
        if (sb.size() > 0) kept.emplace_back(sb);

    std::sort(kept.begin(), kept.end());
    for (const auto& sb : kept)
        std::cout << sb.c_str() << std::endl;
    return 0;
}
