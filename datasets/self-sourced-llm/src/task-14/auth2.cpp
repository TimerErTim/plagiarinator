#include <iostream>

template <typename T>
class Vector {
public:
    Vector() : buf_(nullptr), sz_(0), cap_(0) {}

    ~Vector() { release(); }

    Vector(const Vector& rhs) { copy_from(rhs); }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            release();
            copy_from(rhs);
        }
        return *this;
    }

    void push_back(const T& val) {
        ensure(capacity_needed());
        buf_[sz_++] = val;
    }

    void push_back(T&& val) {
        ensure(capacity_needed());
        buf_[sz_++] = std::move(val);
    }

    T& operator[](std::size_t idx) { return buf_[idx]; }
    const T& operator[](std::size_t idx) const { return buf_[idx]; }

    std::size_t size() const { return sz_; }
    T* data() { return buf_; }
    const T* data() const { return buf_; }

private:
    T* buf_;
    std::size_t sz_, cap_;

    void release() {
        delete[] buf_;
        buf_ = nullptr;
        sz_ = cap_ = 0;
    }

    void copy_from(const Vector& rhs) {
        sz_ = rhs.sz_;
        cap_ = rhs.cap_;
        buf_ = new T[cap_];
        for (std::size_t i = 0; i < sz_; ++i) buf_[i] = rhs.buf_[i];
    }

    std::size_t capacity_needed() const { return sz_ == cap_ ? (cap_ ? cap_ * 2 : 4) : cap_; }

    void ensure(std::size_t need) {
        if (need == cap_) {
            T* next = new T[need];
            for (std::size_t i = 0; i < sz_; ++i) next[i] = buf_[i];
            delete[] buf_;
            buf_ = next;
            cap_ = need;
        }
    }
};

int main() {
    std::size_t count;
    std::cin >> count;
    Vector<int> nums;
    for (std::size_t i = 0; i < count; ++i) {
        int v;
        std::cin >> v;
        nums.push_back(v);
    }

    Vector<int> deduped;
    for (std::size_t i = 0; i < nums.size(); ++i) {
        const int& cur = nums[i];
        bool dup = false;
        for (std::size_t j = 0; j < deduped.size(); ++j) {
            if (deduped[j] == cur) { dup = true; break; }
        }
        if (!dup) deduped.push_back(cur);
    }

    long long s = 0, p = 1;
    constexpr long long M = 1000000007LL;
    for (std::size_t i = 0; i < deduped.size(); ++i) {
        s += deduped[i];
        p = (p * deduped[i]) % M;
    }
    std::cout << "SUM " << s << "\nPROD " << p << '\n';
    return 0;
}

#include <utility>
