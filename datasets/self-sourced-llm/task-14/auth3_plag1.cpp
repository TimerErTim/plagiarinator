#include <iostream>
#include <utility>

template <typename T>
class Vector {
    T* backing;
    std::size_t count;
    std::size_t room;

    void reallocate(std::size_t new_room) {
        T* tmp = new T[new_room];
        for (std::size_t i = 0; i < count; i++) tmp[i] = backing[i];
        delete[] backing;
        backing = tmp;
        room = new_room;
    }

public:
    Vector() : backing(nullptr), count(0), room(0) {}

    ~Vector() { delete[] backing; }

    Vector(const Vector& src) : count(src.count), room(src.room) {
        backing = new T[room];
        for (std::size_t i = 0; i < count; i++) backing[i] = src.backing[i];
    }

    Vector& operator=(const Vector& src) {
        if (this == &src) return *this;
        Vector copy(src);
        swap(copy);
        return *this;
    }

    void swap(Vector& o) {
        std::swap(backing, o.backing);
        std::swap(count, o.count);
        std::swap(room, o.room);
    }

    void push_back(const T& item) {
        if (count >= room) reallocate(room ? room * 2 : 4);
        backing[count++] = item;
    }

    void push_back(T&& item) {
        if (count >= room) reallocate(room ? room * 2 : 4);
        backing[count++] = std::move(item);
    }

    T& operator[](std::size_t at) { return backing[at]; }
    const T& operator[](std::size_t at) const { return backing[at]; }

    std::size_t size() const { return count; }
    T* data() { return backing; }
    const T* data() const { return backing; }
};

bool contains(const Vector<int>& vec, int target) {
    for (std::size_t i = 0; i < vec.size(); i++)
        if (vec[i] == target) return true;
    return false;
}

int main() {
    std::size_t n;
    std::cin >> n;
    Vector<int> input;
    for (std::size_t i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        input.push_back(x);
    }

    Vector<int> filtered;
    for (std::size_t i = 0; i < input.size(); i++) {
        if (!contains(filtered, input[i]))
            filtered.push_back(input[i]);
    }

    long long total = 0, mult = 1;
    const long long MOD = 1'000'000'007LL;
    for (std::size_t i = 0; i < filtered.size(); i++) {
        total += filtered[i];
        mult = (mult * filtered[i]) % MOD;
    }

    std::cout << "SUM " << total << std::endl;
    std::cout << "PROD " << mult << std::endl;
    return 0;
}
