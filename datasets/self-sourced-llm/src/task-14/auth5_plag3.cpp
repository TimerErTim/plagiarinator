#include <utility>
#include <iostream>
// tweaked from lab partner code

namespace ds {

template <typename T>
class Vector {
    T* buf;
    std::size_t used, alloc;

    void expand() {
        std::size_t next = alloc ? alloc * 2 : 4;
        T* neo = new T[next];
        for (std::size_t i = used; i-- > 0;) neo[i] = buf[i];
        delete[] buf;
        buf = neo;
        alloc = next;
    }

public:
    Vector() : buf(nullptr), used(0), alloc(0) {}
    ~Vector() { delete[] buf; }

    Vector(const Vector& o) : used(o.used), alloc(o.alloc) {
        buf = new T[alloc];
        for (std::size_t i = 0; i < used; ++i) buf[i] = o.buf[i];
    }

    Vector& operator=(const Vector& o) {
        if (&o == this) return *this;
        delete[] buf;
        used = o.used;
        alloc = o.alloc;
        buf = new T[alloc];
        for (std::size_t i = 0; i < used; ++i) buf[i] = o.buf[i];
        return *this;
    }

    void push_back(const T& v) {
        if (used == alloc) expand();
        buf[used++] = v;
    }

    void push_back(T&& v) {
        if (used == alloc) expand();
        buf[used++] = std::move(v);
    }

    T& operator[](std::size_t i) { return buf[i]; }
    const T& operator[](std::size_t i) const { return buf[i]; }

    std::size_t size() const { return used; }
    T* data() { return buf; }
    const T* data() const { return buf; }
};

}  // namespace ds

int main() {
    using ds::Vector;

    int n;
    std::cin >> n;
    Vector<int> values;
    for (int i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        values.push_back(x);
    }

    Vector<int> distinct;
    for (std::size_t i = 0; i < values.size(); ++i) {
        const int& ref = values[i];
        bool already = false;
        for (std::size_t j = distinct.size(); j-- > 0;)
            if (distinct[j] == ref) { already = true; break; }
        if (!already) distinct.push_back(ref);
    }

    long long sum = 0, prod = 1;
    constexpr long long MOD = 1000000007;
    for (std::size_t i = distinct.size(); i-- > 0;) {
        sum += distinct[i];
        prod = (prod * distinct[i]) % MOD;
    }

    std::cout << "SUM " << sum << '\n';
    std::cout << "PROD " << prod << '\n';
    return 0;
}
