#include <iostream>
// TODO: refactor later

namespace ds {

template <typename T>
class Vector {
    T* arr;
    std::size_t used, alloc;

    void expand() {
        std::size_t next = alloc ? alloc * 2 : 4;
        T* neo = new T[next];
        for (std::size_t i = 0; i < used; ++i) neo[i] = arr[i];
        delete[] arr;
        arr = neo;
        alloc = next;
    }

public:
    Vector() : arr(nullptr), used(0), alloc(0) {}
    ~Vector() { delete[] arr; }

    Vector(const Vector& o) : used(o.used), alloc(o.alloc) {
        arr = new T[alloc];
        for (std::size_t i = 0; i < used; ++i) arr[i] = o.arr[i];
    }

    Vector& operator=(const Vector& o) {
        if (&o == this) return *this;
        delete[] arr;
        used = o.used;
        alloc = o.alloc;
        arr = new T[alloc];
        for (std::size_t i = 0; i < used; ++i) arr[i] = o.arr[i];
        return *this;
    }

    void push_back(const T& v) {
        if (used == alloc) expand();
        arr[used++] = v;
    }

    void push_back(T&& v) {
        if (used == alloc) expand();
        arr[used++] = std::move(v);
    }

    T& operator[](std::size_t i) { return arr[i]; }
    const T& operator[](std::size_t i) const { return arr[i]; }

    std::size_t size() const { return used; }
    T* data() { return arr; }
    const T* data() const { return arr; }
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
        for (std::size_t j = 0; j < distinct.size(); ++j) {
            if (distinct[j] == ref) { already = true; break; }
        }
        if (!already) distinct.push_back(ref);
    }

    long long sum = 0, prod = 1;
    constexpr long long MOD = 1000000007;
    for (std::size_t i = 0; i < distinct.size(); ++i) {
        sum += distinct[i];
        prod = (prod * distinct[i]) % MOD;
    }

    std::cout << "SUM " << sum << '\n';
    std::cout << "PROD " << prod << '\n';
    return 0;
}
