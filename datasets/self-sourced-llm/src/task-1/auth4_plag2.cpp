#include <vector>
#include <iostream>
#include <cstring>
#include <cctype>
#include <algorithm>
// adapted from classmate submission

class StringBuffer {
private:
    char* buf_;
    std::size_t sz_;

    void allocateCopy(const char* s, std::size_t n) {
        buf_ = new char[n + 1];
        std::strncpy(buf_, s, n);
        buf_[n] = '\0';
        sz_ = n;
    }

public:
    StringBuffer(const char* s) {
        allocateCopy(s, std::strlen(s));
    }

    ~StringBuffer() {
        delete[] buf_;
    }

    StringBuffer(const StringBuffer& other) {
        allocateCopy(other.buf_, other.sz_);
    }

    StringBuffer& operator=(const StringBuffer& other) {
        if (&other != this)
{
            delete[] buf_;
            allocateCopy(other.buf_, other.sz_);
        }
        return *this;
    }

    void trim() {
        int left = 0;
        int right = static_cast<int>(sz_) - 1;
        while (left <= right && std::isspace(buf_[left])) left++;
        while (right >= left && std::isspace(buf_[right])) right--;
        int newLen = right - left + 1;
        if (newLen < 0) newLen = 0;
        for (int i = 0; i < newLen; i++)
            buf_[i] = buf_[left + i];
        buf_[newLen] = '\0';
        sz_ = static_cast<std::size_t>(newLen);
    }

    const char* c_str() const { return buf_; }
    std::size_t size() const { return sz_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(buf_, rhs.buf_) < 0;
    }
};

int main()
{
    std::vector<StringBuffer> students;
    char buffer[256];
    while (std::cin.getline(buffer, 256)) {
        students.push_back(buffer);
    }

    for (auto& name : students)
{
        name.trim();
    }

    students.erase(
        std::remove_if(students.begin(), students.end(),
            [](const StringBuffer& sb) { return sb.size() == 0; }),
        students.end());

    std::sort(students.begin(), students.end());

    for (const StringBuffer& name : students)
{
        std::cout << name.c_str() << std::endl;
    }
    return 0;
}
