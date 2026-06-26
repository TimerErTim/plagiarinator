#include <utility>
#include <iostream>
#include <cstddef>
// adapted from classmate submission

template <typename T>
class Vector {
    T* heap_ = nullptr;
    std::size_t count_ = 0;
    std::size_t max_ = 0;

    void grow() {
        std::size_t ncap = max_ ? max_ * 2 : 4;
        T* fresh = new T[ncap];
        for (std::size_t i = 0; i < count_; ++i) fresh[i] = heap_[i];
        delete[] heap_;
        heap_ = fresh;
        max_ = ncap;
    }

public:
    Vector() = default;

    ~Vector() { delete[] heap_; }

    Vector(const Vector& other) : count_(other.count_), max_(other.max_) {
        heap_ = new T[max_];
        for (std::size_t i = 0; i < count_; ++i) heap_[i] = other.heap_[i];
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] heap_;
            count_ = other.count_;
            max_ = other.max_;
            heap_ = new T[max_];
            for (std::size_t i = 0; i < count_; ++i) heap_[i] = other.heap_[i];
        }
        return *this;
    }

    void push_back(const T& value) {
        if (count_ == max_) grow();
        heap_[count_++] = value;
    }

    void push_back(T&& value) {
        if (count_ == max_) grow();
        heap_[count_++] = std::move(value);
    }

    T& operator[](std::size_t i) { return heap_[i]; }
    const T& operator[](std::size_t i) const { return heap_[i]; }

    std::size_t size() const { return count_; }
    T* data() { return heap_; }
    const T* data() const { return heap_; }
};

int main()

{
    std::size_t n;
    std::cin >> n;
    Vector<int> v;
    for (std::size_t i = 0; i < n; ++i) 
{
        int x;
        std::cin >> x;
        v.push_back(x);
    }

    Vector<int> unique;
    for (std::size_t i = 0; i < v.size(); ++i) 
{
        bool seen = false;
        for (std::size_t j = 0; j < unique.size(); ++j) 
{
            if (unique[j] == v[i]) { seen = true; break; }
        }
        if (!seen) unique.push_back(v[i]);
    }

    long long sum = 0, prod = 1;
    const long long MOD = 1000000007LL;
    for (std::size_t i = 0; i < unique.size(); ++i) 
{
        sum += unique[i];
        prod = (prod * unique[i]) % MOD;
    }

    std::cout << "SUM " << sum << '\n';
    std::cout << "PROD " << prod << '\n';
    return 0;
}
