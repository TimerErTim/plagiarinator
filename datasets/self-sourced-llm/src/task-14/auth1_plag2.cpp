#include <cstddef>
#include <iostream>
#include <utility>

template <typename T>
class Vector {
    T* storage_ = nullptr;
    std::size_t n_ = 0;
    std::size_t cap_ = 0;

    void grow()
    {
        std::size_t ncap = cap_ ? cap_ * 2 : 4;
        T* fresh = new T[ncap];
        for (std::size_t i = 0; i < n_; ++i) fresh[i] = storage_[i];
        delete[] storage_;
        storage_ = fresh;
        cap_ = ncap;
    }

public:
    Vector() = default;
    ~Vector() { delete[] storage_; }

    Vector(const Vector& other) : n_(other.n_), cap_(other.cap_) {
        storage_ = new T[cap_];
        for (std::size_t i = 0; i < n_; ++i) storage_[i] = other.storage_[i];
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] storage_;
            n_ = other.n_;
            cap_ = other.cap_;
            storage_ = new T[cap_];
            for (std::size_t i = 0; i < n_; ++i) storage_[i] = other.storage_[i];
        }
        return *this;
    }

    void push_back(const T& value) {
        if (n_ == cap_) grow();
        storage_[n_++] = value;
    }

    void push_back(T&& value) {
        if (n_ == cap_) grow();
        storage_[n_++] = std::move(value);
    }

    T& operator[](std::size_t i) { return storage_[i]; }
    const T& operator[](std::size_t i) const { return storage_[i]; }

    std::size_t size() const { return n_; }
    T* data() { return storage_; }
    const T* data() const { return storage_; }
};

static void read_values(Vector<int>& v, std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        v.push_back(x);
    }
}

static void dedupe(const Vector<int>& src, Vector<int>& out)
{
    for (std::size_t i = 0; i < src.size(); ++i) {
        bool seen = false;
        for (std::size_t j = 0; j < out.size(); ++j) {
            if (out[j] == src[i]) { seen = true; break; }
        }
        if (!seen) out.push_back(src[i]);
    }
}

int main()
{
    std::size_t n;
    std::cin >> n;
    Vector<int> v;
    read_values(v, n);

    Vector<int> unique;
    dedupe(v, unique);

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
