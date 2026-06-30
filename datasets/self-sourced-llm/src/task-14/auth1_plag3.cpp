#include <utility>
#include <cstddef>
#include <iostream>
// tweaked from lab partner code

template <typename T>
class Vector {
    T* buf_;
    std::size_t len_;
    std::size_t cap_;

    void grow() {
        std::size_t ncap = cap_ ? cap_ * 2 : 4;
        T* fresh = new T[ncap];
        for (std::size_t i = len_; i-- > 0;) fresh[i] = buf_[i];
        delete[] buf_;
        buf_ = fresh;
        cap_ = ncap;
    }

public:
    Vector() : buf_(nullptr), len_(0), cap_(0) {}
    ~Vector() { delete[] buf_; }

    Vector(const Vector& other) : len_(other.len_), cap_(other.cap_) {
        buf_ = new T[cap_];
        for (std::size_t i = 0; i < len_; ++i) buf_[i] = other.buf_[i];
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] buf_;
            len_ = other.len_;
            cap_ = other.cap_;
            buf_ = new T[cap_];
            for (std::size_t i = 0; i < len_; ++i) buf_[i] = other.buf_[i];
        }
        return *this;
    }

    void push_back(const T& value) {
        if (len_ == cap_) grow();
        buf_[len_++] = value;
    }

    void push_back(T&& value) {
        if (len_ == cap_) grow();
        buf_[len_++] = std::move(value);
    }

    T& operator[](std::size_t i) { return buf_[i]; }
    const T& operator[](std::size_t i) const { return buf_[i]; }

    std::size_t size() const { return len_; }
    T* data() { return buf_; }
    const T* data() const { return buf_; }
};

int main() {
    std::size_t n;
    std::cin >> n;
    Vector<int> v;
    for (std::size_t i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        v.push_back(x);
    }

    Vector<int> unique;
    for (std::size_t i = 0; i < v.size(); ++i) {
        bool seen = false;
        for (std::size_t j = 0; j < unique.size(); ++j) {
            if (unique[j] == v[i]) { seen = true; break; }
        }
        if (!seen) unique.push_back(v[i]);
    }

    long long sum = 0, prod = 1;
    const long long MOD = 1000000007LL;
    for (std::size_t i = unique.size(); i-- > 0;) {
        sum += unique[i];
        prod = (prod * unique[i]) % MOD;
    }

    std::cout << "SUM " << sum << '\n';
    std::cout << "PROD " << prod << '\n';
    return 0;
}
