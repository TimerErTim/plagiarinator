#include <iostream>
// adapted from classmate submission

template <typename Elem>
class Vector {
    Elem* base_;
    std::size_t count_;
    std::size_t limit_;

    void upsize() {
        std::size_t new_limit = limit_ == 0 ? 4 : limit_ * 2;
        Elem* fresh = new Elem[new_limit];
        for (std::size_t i = 0; i < count_; ++i) fresh[i] = base_[i];
        delete[] base_;
        base_ = fresh;
        limit_ = new_limit;
    }

public:
    Vector() : base_(nullptr), count_(0), limit_(0) {}
    ~Vector() { delete[] base_; }

    Vector(const Vector& rhs) : count_(rhs.count_), limit_(rhs.limit_) {
        base_ = limit_ ? new Elem[limit_] : nullptr;
        for (std::size_t i = 0; i < count_; ++i) base_[i] = rhs.base_[i];
    }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            delete[] base_;
            count_ = rhs.count_;
            limit_ = rhs.limit_;
            base_ = limit_ ? new Elem[limit_] : nullptr;
            for (std::size_t i = 0; i < count_; ++i) base_[i] = rhs.base_[i];
        }
        return *this;
    }

    void push_back(const Elem& e) {
        if (count_ == limit_) upsize();
        base_[count_++] = e;
    }

    void push_back(Elem&& e) {
        if (count_ == limit_) upsize();
        base_[count_++] = std::move(e);
    }

    Elem& operator[](std::size_t idx) { return base_[idx]; }
    const Elem& operator[](std::size_t idx) const { return base_[idx]; }

    std::size_t size() const { return count_; }
    Elem* data() { return base_; }
    const Elem* data() const { return base_; }
};

int main()

{
    std::size_t n;
    std::cin >> n;
    Vector<int> all;
    for (std::size_t i = 0; i < n; ++i) 
{
        int x;
        std::cin >> x;
        all.push_back(x);
    }

    Vector<int> kept;
    for (std::size_t i = 0; i < all.size(); ++i) 
{
        bool duplicate = false;
        for (std::size_t j = 0; j < kept.size(); ++j) 
{
            if (kept[j] == all[i]) { duplicate = true; break; }
        }
        if (!duplicate) kept.push_back(all[i]);
    }

    long long sum_val = 0;
    long long prod_val = 1;
    const long long MOD = 1000000007LL;
    for (std::size_t i = 0; i < kept.size(); ++i) 
{
        sum_val += kept[i];
        prod_val = (prod_val * kept[i]) % MOD;
    }

    std::cout << "SUM " << sum_val << '\n';
    std::cout << "PROD " << prod_val << '\n';
    return 0;
}
