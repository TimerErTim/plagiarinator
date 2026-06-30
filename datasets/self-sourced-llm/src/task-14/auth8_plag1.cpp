#include <iostream>
#include <utility>

template <typename T>
class Vector {
    T* backing;
    std::size_t len, cap;

public:
    Vector() : backing(nullptr), len(0), cap(0) {}
    ~Vector() { delete[] backing; }

    Vector(const Vector& v) {
        len = v.len;
        cap = v.cap;
        backing = new T[cap];
        for (std::size_t i = 0; i < len; i++) backing[i] = v.backing[i];
    }

    Vector& operator=(const Vector& v) {
        if (this == &v) return *this;
        T* new_mem = new T[v.cap];
        for (std::size_t i = 0; i < v.len; i++) new_mem[i] = v.backing[i];
        delete[] backing;
        backing = new_mem;
        len = v.len;
        cap = v.cap;
        return *this;
    }

    void push_back(const T& x) {
        ensure(len + 1);
        backing[len++] = x;
    }

    void push_back(T&& x) {
        ensure(len + 1);
        backing[len++] = std::move(x);
    }

    T& operator[](std::size_t i) { return backing[i]; }
    const T& operator[](std::size_t i) const { return backing[i]; }

    std::size_t size() const { return len; }
    T* data() { return backing; }
    const T* data() const { return backing; }

private:
    void ensure(std::size_t need) {
        if (need <= cap) return;
        std::size_t nc = cap ? cap * 2 : 4;
        while (nc < need) nc *= 2;
        T* nm = new T[nc];
        for (std::size_t i = 0; i < len; i++) nm[i] = backing[i];
        delete[] backing;
        backing = nm;
        cap = nc;
    }
};

int main() {
    std::size_t n;
    std::cin >> n;
    Vector<int> nums;
    for (std::size_t i = 0; i < n; i++) {
        int v;
        std::cin >> v;
        nums.push_back(v);
    }

    Vector<int> result;
    for (std::size_t i = 0; i < nums.size(); i++) {
        const int& val = nums[i];
        bool skip = false;
        for (std::size_t j = 0; j < result.size(); j++) {
            if (result[j] == val) { skip = true; break; }
        }
        if (!skip) result.push_back(val);
    }

    long long s = 0, p = 1;
    const long long MOD = 1e9 + 7;
    for (std::size_t i = 0; i < result.size(); i++) {
        s += result[i];
        p = (p * result[i]) % MOD;
    }

    std::cout << "SUM " << s << std::endl;
    std::cout << "PROD " << p << std::endl;
    return 0;
}
