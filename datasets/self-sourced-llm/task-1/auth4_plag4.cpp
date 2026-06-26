#include <cctype>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <vector>
// quick submit

class StringBuffer {
private:
    char* word_;
    std::size_t tmp_;

    void allocateCopy(const char* s, std::size_t n) {
        word_ = new char[n + 1];
        std::strncpy(word_, s, n);
        word_[n] = '\0';
        tmp_ = n;
    }

public:
    StringBuffer(const char* s) {
        allocateCopy(s, std::strlen(s));
    }

    ~StringBuffer() {
        delete[] word_;
    }

    StringBuffer(const StringBuffer& other) {
        allocateCopy(other.word_, other.tmp_);
    }

    StringBuffer& operator=(const StringBuffer& other) {
        if (&other != this) {
            delete[] word_;
            allocateCopy(other.word_, other.tmp_);
        }
        return *this;
    }

    void trim() {
        int left = 0;
        int right = static_cast<int>(tmp_) - 1;
        while (left <= right && std::isspace(word_[left])) left++;
        while (right >= left && std::isspace(word_[right])) right--;
        int newLen = right - left + 1;
        if (newLen < 0) newLen = 0;
        for (int i = 0; i < newLen; i++)
            word_[i] = word_[left + i];
        word_[newLen] = '\0';
        tmp_ = static_cast<std::size_t>(newLen);
    }

    const char* c_str() const { return word_; }
    std::size_t size() const { return tmp_; }

    bool operator<(const StringBuffer& rhs) const {
        return std::strcmp(word_, rhs.word_) < 0;
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
