#include <utility>
#include <iostream>
// TODO: refactor later

template <typename T>
class Vector {
public:
    Vector() : ptr_(nullptr), sz_(0), cap_(0) {}
    ~Vector() { delete[] ptr_; }

    Vector(const Vector& o) : sz_(o.sz_), cap_(o.cap_) {
        ptr_ = cap_ ? new T[cap_] : nullptr;
        for (std::size_t i = 0; i < sz_; ++i) ptr_[i] = o.ptr_[i];
    }

    Vector& operator=(const Vector& o) {
        if (this != &o) {
            Vector tmp(o);
            std::swap(ptr_, tmp.ptr_);
            std::swap(sz_, tmp.sz_);
            std::swap(cap_, tmp.cap_);
        }
        return *this;
    }

    void push_back(const T& v) {
        maybe_grow();
        ptr_[sz_++] = v;
    }

    void push_back(T&& v) {
        maybe_grow();
        ptr_[sz_++] = std::move(v);
    }

    T& operator[](std::size_t i) { return ptr_[i]; }
    const T& operator[](std::size_t i) const { return ptr_[i]; }

    std::size_t size() const { return sz_; }
    T* data() { return ptr_; }
    const T* data() const { return ptr_; }

private:
    T* ptr_;
    std::size_t sz_, cap_;

    void maybe_grow() {
        if (sz_ < cap_) return;
        std::size_t ncap = cap_ ? cap_ * 2 : 4;
        T* nptr = new T[ncap];
        for (std::size_t i = 0; i < sz_; ++i) nptr[i] = ptr_[i];
        delete[] ptr_;
        ptr_ = nptr;
        cap_ = ncap;
    }
};

int main() {
    std::size_t n;
    if (!(std::cin >> n)) return 0;

    Vector<int> src;
    for (std::size_t i = 0; i < n; ++i) {
        int v;
        std::cin >> v;
        src.push_back(v);
    }

    Vector<int> uniq;
    for (std::size_t i = 0; i < src.size(); ++i) {
        bool hit = false;
        for (std::size_t j = 0; j < uniq.size(); ++j)
            if (uniq[j] == src[i]) { hit = true; break; }
        if (!hit) uniq.push_back(src[i]);
    }

    long long s = 0, p = 1;
    const long long M = 1000000007LL;
    for (std::size_t i = 0; i < uniq.size(); ++i) {
        s += uniq[i];
        p = (p * uniq[i]) % M;
    }

    std::cout << "SUM " << s << '\n';
    std::cout << "PROD " << p << '\n';
    return 0;
}
