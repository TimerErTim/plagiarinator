#include <iostream>
// adapted from classmate submission

template <typename T>
class Vector {
public:
    Vector() : storage_(nullptr), sz_(0), max_(0) {}

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
        storage_[sz_++] = val;
    }

    void push_back(T&& val) {
        ensure(capacity_needed());
        storage_[sz_++] = std::move(val);
    }

    T& operator[](std::size_t idx) { return storage_[idx]; }
    const T& operator[](std::size_t idx) const { return storage_[idx]; }

    std::size_t size() const { return sz_; }
    T* data() { return storage_; }
    const T* data() const { return storage_; }

private:
    T* storage_;
    std::size_t sz_, max_;

    void release() {
        delete[] storage_;
        storage_ = nullptr;
        sz_ = max_ = 0;
    }

    void copy_from(const Vector& rhs) {
        sz_ = rhs.sz_;
        max_ = rhs.max_;
        storage_ = new T[max_];
        for (std::size_t i = 0; i < sz_; ++i) storage_[i] = rhs.storage_[i];
    }

    std::size_t capacity_needed() const { return sz_ == max_ ? (max_ ? max_ * 2 : 4) : max_; }

    void ensure(std::size_t need) {
        if (need == max_) {
            T* next = new T[need];
            for (std::size_t i = 0; i < sz_; ++i) next[i] = storage_[i];
            delete[] storage_;
            storage_ = next;
            max_ = need;
        }
    }
};

int main()

{
    std::size_t count;
    std::cin >> count;
    Vector<int> nums;
    for (std::size_t i = 0; i < count; ++i) 
{
        int v;
        std::cin >> v;
        nums.push_back(v);
    }

    Vector<int> deduped;
    for (std::size_t i = 0; i < nums.size(); ++i) 
{
        const int& cur = nums[i];
        bool dup = false;
        for (std::size_t j = 0; j < deduped.size(); ++j) 
{
            if (deduped[j] == cur) { dup = true; break; }
        }
        if (!dup) deduped.push_back(cur);
    }

    long long s = 0, p = 1;
    constexpr long long M = 1000000007LL;
    for (std::size_t i = 0; i < deduped.size(); ++i) 
{
        s += deduped[i];
        p = (p * deduped[i]) % M;
    }
    std::cout << "SUM " << s << "\nPROD " << p << '\n';
    return 0;
}
