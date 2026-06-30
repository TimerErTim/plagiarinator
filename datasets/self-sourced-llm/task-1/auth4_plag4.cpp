#include <cstring>
#include <cctype>
#include <vector>
#include <iostream>
#include <algorithm>

class StringBuffer {
private:
    char* m_data;
    std::size_t m_len;

    void allocateCopy(const char* s, std::size_t n) {
        m_data = new char[n + 1];
        std::strncpy(m_data, s, n);
        m_data[n] = '\0';
        m_len = n;
    }

public:
    StringBuffer(const char* s) { allocateCopy(s, std::strlen(s)); }
    ~StringBuffer() { delete[] m_data; }
    StringBuffer(const StringBuffer& other) { allocateCopy(other.m_data, other.m_len); }
    StringBuffer& operator=(const StringBuffer& other) {
        if (&other != this) {
            delete[] m_data;
            allocateCopy(other.m_data, other.m_len);
        }
        return *this;
    }

    void trim() {
        int left = 0;
        int right = static_cast<int>(m_len) - 1;
        while (left <= right && std::isspace(m_data[left])) left++;
        while (right >= left && std::isspace(m_data[right])) right--;
        int newLen = right - left + 1;
        if (newLen < 0) newLen = 0;
        for (int i = 0; i < newLen; i++)
            m_data[i] = m_data[left + i];
        m_data[newLen] = '\0';
        m_len = static_cast<std::size_t>(newLen);
    }

    const char* c_str() const { return m_data; }
    std::size_t size() const { return m_len; }
    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(m_data, rhs.m_data) < 0;
    }
};

int main() {
    std::vector<StringBuffer> rows;
    char buffer[256];
    while (std::cin.getline(buffer, 256))
        rows.emplace_back(buffer);

    for (auto& row : rows) row.trim();
    rows.erase(std::remove_if(rows.begin(), rows.end(),
            [](const StringBuffer& sb) { return sb.size() == 0; }), rows.end());
    std::sort(rows.begin(), rows.end());
    for (const StringBuffer& row : rows)
        std::cout << row.c_str() << '\n';
    return 0;
}
