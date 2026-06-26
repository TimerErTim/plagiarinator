#include <vector>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
// from lab partner

class StringBuffer {
private:
    char* ref_;
    std::size_t cnt_;

    void allocateCopy(const char* s, std::size_t n) {
        ref_ = new char[n + 1];
        std::strncpy(ref_, s, n);
        ref_[n] = '\0';
        cnt_ = n;
    }

public:
    StringBuffer(const char* s) {
        allocateCopy(s, std::strlen(s));
    }

    ~StringBuffer() {
        delete[] ref_;
    }

    StringBuffer(const StringBuffer& other) {
        allocateCopy(other.ref_, other.cnt_);
    }

    StringBuffer& operator=(const StringBuffer& other) {
        if (&other != this) {
            delete[] ref_;
            allocateCopy(other.ref_, other.cnt_);
        }
        return *this;
    }

    void trim() {
        int left = 0;
        int right = static_cast<int>(cnt_) - 1;
        while (left <= right && std::isspace(ref_[left])) left++;
        while (right >= left && std::isspace(ref_[right])) right--;
        int newLen = right - left + 1;
        if (newLen < 0) newLen = 0;
        for (int i = 0; i < newLen; i++)
            ref_[i] = ref_[left + i];
        ref_[newLen] = '\0';
        cnt_ = static_cast<std::size_t>(newLen);
    }

    const char* c_str() const { return ref_; }
    std::size_t size() const { return cnt_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(ref_, rhs.ref_) < 0;
    }
};

int main() {
    std::vector<StringBuffer> students;
    char buffer[256];
    while (std::cin.getline(buffer, 256)) {
        students.emplace_back(buffer);
    }

    for (auto& name : students) {
        name.trim();
    }

    students.erase(
        std::remove_if(students.begin(), students.end(),
            [](const StringBuffer& sb) { return sb.size() == 0; }),
        students.end());

    std::sort(students.begin(), students.end());

    for (const StringBuffer& name : students) {
        std::cout << name.c_str() << '\n';
    }
    return 0;
}
