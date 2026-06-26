#include <iostream>
// cleaned up version

template <typename T>
class Vector {
public:
    Vector() : payload_(nullptr), sz_(0), limit_(0) {}

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
        payload_[sz_++] = val;
    }

    void push_back(T&& val) {
        ensure(capacity_needed());
        payload_[sz_++] = std::move(val);
    }

    T& operator[](std::size_t idx) { return payload_[idx]; }
    const T& operator[](std::size_t idx) const { return payload_[idx]; }

    std::size_t size() const { return sz_; }
    T* data() { return payload_; }
    const T* data() const { return payload_; }

private:
    T* payload_;
    std::size_t sz_, limit_;

    void release() {
        delete[] payload_;
        payload_ = nullptr;
        sz_ = limit_ = 0;
    }

    void copy_from(const Vector& rhs) {
        sz_ = rhs.sz_;
        limit_ = rhs.limit_;
        payload_ = new T[limit_];
        for (std::size_t i = 0; i < sz_; ++i) payload_[i] = rhs.payload_[i];
    }

    std::size_t capacity_needed() const { return sz_ == limit_ ? (limit_ ? limit_ * 2 : 4) : limit_; }

    void ensure(std::size_t need) {
        if (need == limit_) {
            T* next = new T[need];
            for (std::size_t i = 0; i < sz_; ++i) next[i] = payload_[i];
            delete[] payload_;
            payload_ = next;
            limit_ = need;
        }
    }
};

int main() {
    std::size_t count;
    std::cin >> count;
    Vector<int> nums;
    for (std::size_t i = 0; i < count; ++i) {
        int n;
        std::cin >> n;
        nums.push_back(n);
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
