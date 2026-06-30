#include <iostream>
#include <utility>

namespace ds {

template <typename T>
class Vector {
    T* elems;
    std::size_t used, alloc;

    void expand() {
        std::size_t next = alloc ? alloc * 2 : 4;
        T* neo = new T[next];
        for (std::size_t i = 0; i < used; i++) neo[i] = elems[i];
        delete[] elems;
        elems = neo;
        alloc = next;
    }

public:
    Vector() : elems(nullptr), used(0), alloc(0) {}
    ~Vector() { delete[] elems; }

    Vector(const Vector& o) : used(o.used), alloc(o.alloc) {
        elems = new T[alloc];
        for (std::size_t i = 0; i < used; i++) elems[i] = o.elems[i];
    }

    Vector& operator=(const Vector& o) {
        if (&o == this) return *this;
        delete[] elems;
        used = o.used;
        alloc = o.alloc;
        elems = new T[alloc];
        for (std::size_t i = 0; i < used; i++) elems[i] = o.elems[i];
        return *this;
    }

    void push_back(const T& v) {
        if (used == alloc) expand();
        elems[used++] = v;
    }

    void push_back(T&& v) {
        if (used == alloc) expand();
        elems[used++] = std::move(v);
    }

    T& operator[](std::size_t i) { return elems[i]; }
    const T& operator[](std::size_t i) const { return elems[i]; }

    std::size_t size() const { return used; }
    T* data() { return elems; }
    const T* data() const { return elems; }
};

}  // namespace ds

int main() {
    using ds::Vector;

    int n;
    std::cin >> n;
    Vector<int> values;
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        values.push_back(x);
    }

    Vector<int> distinct;
    for (std::size_t i = 0; i < values.size(); i++) {
        const int& ref = values[i];
        bool already = false;
        for (std::size_t j = 0; j < distinct.size(); j++) {
            if (distinct[j] == ref) { already = true; break; }
        }
        if (!already) distinct.push_back(ref);
    }

    long long sum = 0, prod = 1;
    constexpr long long MOD = 1000000007;
    for (std::size_t i = 0; i < distinct.size(); i++) {
        sum += distinct[i];
        prod = (prod * distinct[i]) % MOD;
    }

    std::cout << "SUM " << sum << std::endl;
    std::cout << "PROD " << prod << std::endl;
    return 0;
}
