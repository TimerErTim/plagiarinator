#include <iostream>
#include <algorithm>
// adapted from classmate submission

template <typename T>
class Vector {
    T* heap;
    std::size_t n, cap;

    void reserve_more() {
        std::size_t new_cap = cap ? cap * 2 : 4;
        T* bigger = new T[new_cap];
        std::copy(heap, heap + n, bigger);
        delete[] heap;
        heap = bigger;
        cap = new_cap;
    }

public:
    Vector() : heap(nullptr), n(0), cap(0) {}
    ~Vector() { delete[] heap; }

    Vector(const Vector& v) : n(v.n), cap(v.cap) {
        heap = new T[cap];
        std::copy(v.heap, v.heap + n, heap);
    }

    Vector& operator=(Vector v) {
        swap(v);
        return *this;
    }

    void swap(Vector& o) {
        std::swap(heap, o.heap);
        std::swap(n, o.n);
        std::swap(cap, o.cap);
    }

    void push_back(const T& x) {
        if (n == cap) reserve_more();
        heap[n++] = x;
    }

    void push_back(T&& x) {
        if (n == cap) reserve_more();
        heap[n++] = std::move(x);
    }

    T& operator[](std::size_t i) { return heap[i]; }
    const T& operator[](std::size_t i) const { return heap[i]; }

    std::size_t size() const { return n; }
    T* data() { return heap; }
    const T* data() const { return heap; }
};

int main()

{
    std::size_t k;
    std::cin >> k;
    Vector<int> raw;
    for (std::size_t i = 0; i < k; ++i) 
{
        int x;
        std::cin >> x;
        raw.push_back(x);
    }

    Vector<int> out;
    for (std::size_t i = 0; i < raw.size(); ++i) 
{
        bool found = false;
        for (std::size_t j = 0; j < out.size(); ++j) 
{
            if (out[j] == raw[i]) { found = true; break; }
        }
        if (!found) out.push_back(raw[i]);
    }

    long long sum = 0;
    long long prod = 1;
    const long long P = 1000000007LL;
    for (std::size_t i = 0; i < out.size(); ++i) 
{
        sum += out[i];
        prod = (prod * out[i]) % P;
    }

    std::cout << "SUM " << sum << '\n';
    std::cout << "PROD " << prod << '\n';
    return 0;
}
