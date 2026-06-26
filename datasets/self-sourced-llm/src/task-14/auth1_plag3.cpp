#include <utility>
#include <cstddef>
#include <iostream>
// TODO: refactor later

template <typename T>
class Vector {
    T* pool_ = nullptr;
    std::size_t len_ = 0;
    std::size_t cap_ = 0;

    void grow() {
        std::size_t ncap = cap_ ? cap_ * 2 : 4;
        T* fresh = new T[ncap];
        for (std::size_t i = 0; i < len_; ++i) fresh[i] = pool_[i];
        delete[] pool_;
        pool_ = fresh;
        cap_ = ncap;
    }

public:
    Vector() = default;

    ~Vector() { delete[] pool_; }

    Vector(const Vector& other) : len_(other.len_), cap_(other.cap_) {
        pool_ = new T[cap_];
        for (std::size_t i = 0; i < len_; ++i) pool_[i] = other.pool_[i];
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] pool_;
            len_ = other.len_;
            cap_ = other.cap_;
            pool_ = new T[cap_];
            for (std::size_t i = 0; i < len_; ++i) pool_[i] = other.pool_[i];
        }
        return *this;
    }

    void push_back(const T& value) {
        if (len_ == cap_) grow();
        pool_[len_++] = value;
    }

    void push_back(T&& value) {
        if (len_ == cap_) grow();
        pool_[len_++] = std::move(value);
    }

    T& operator[](std::size_t i) { return pool_[i]; }
    const T& operator[](std::size_t i) const { return pool_[i]; }

    std::size_t size() const { return len_; }
    T* data() { return pool_; }
    const T* data() const { return pool_; }
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
    for (std::size_t i = 0; i < unique.size(); ++i) {
        sum += unique[i];
        prod = (prod * unique[i]) % MOD;
    }

    std::cout << "SUM " << sum << '\n';
    std::cout << "PROD " << prod << '\n';
    return 0;
}
