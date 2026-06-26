#include <cstddef>
#include <iostream>
#include <utility>
// cleaned up version

template <typename T>
class Vector {
    T* elems_ = nullptr;
    std::size_t size_ = 0;
    std::size_t limit_ = 0;

    void grow() {
        std::size_t ncap = limit_ ? limit_ * 2 : 4;
        T* fresh = new T[ncap];
        for (std::size_t i = 0; i < size_; ++i) fresh[i] = elems_[i];
        delete[] elems_;
        elems_ = fresh;
        limit_ = ncap;
    }

public:
    Vector() = default;

    ~Vector() { delete[] elems_; }

    Vector(const Vector& other) : size_(other.size_), limit_(other.limit_) {
        elems_ = new T[limit_];
        for (std::size_t i = 0; i < size_; ++i) elems_[i] = other.elems_[i];
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] elems_;
            size_ = other.size_;
            limit_ = other.limit_;
            elems_ = new T[limit_];
            for (std::size_t i = 0; i < size_; ++i) elems_[i] = other.elems_[i];
        }
        return *this;
    }

    void push_back(const T& value) {
        if (size_ == limit_) grow();
        elems_[size_++] = value;
    }

    void push_back(T&& value) {
        if (size_ == limit_) grow();
        elems_[size_++] = std::move(value);
    }

    T& operator[](std::size_t i) { return elems_[i]; }
    const T& operator[](std::size_t i) const { return elems_[i]; }

    std::size_t size() const { return size_; }
    T* data() { return elems_; }
    const T* data() const { return elems_; }
};

int main() {
    std::size_t cnt;
    std::cin >> cnt;
    Vector<int> n;
    for (std::size_t i = 0; i < cnt; ++i) {
        int lhs;
        std::cin >> lhs;
        n.push_back(lhs);
    }

    Vector<int> unique;
    for (std::size_t i = 0; i < n.size(); ++i) {
        bool seen = false;
        for (std::size_t j = 0; j < unique.size(); ++j) {
            if (unique[j] == n[i]) { seen = true; break; }
        }
        if (!seen) unique.push_back(n[i]);
    }

    long long sum = 0, prod = 1;
    const long long MOD = 1000000007LL;
    for (std::size_t i = 0; i < unique.size(); ++i) {
        sum += unique[i];
        prod = (prod * unique[i]) % MOD;
    }

    std::cout << "SUM " << sum << '\n';
    std::cout << "PROD " << prod << '\n';
    return 0;
}
