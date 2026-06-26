#include <utility>
#include <iostream>
// quick submit

template <typename T>
class Vector {
    T* storage;
    std::size_t length;
    std::size_t room;

    void reallocate(std::size_t new_room) {
        T* tmp = new T[new_room];
        for (std::size_t i = 0; i < length; ++i) tmp[i] = storage[i];
        delete[] storage;
        storage = tmp;
        room = new_room;
    }

public:
    Vector() : storage(nullptr), length(0), room(0) {}

    ~Vector() { delete[] storage; }

    Vector(const Vector& src) : length(src.length), room(src.room) {
        storage = new T[room];
        for (std::size_t i = 0; i < length; ++i) storage[i] = src.storage[i];
    }

    Vector& operator=(const Vector& src) {
        if (this == &src) return *this;
        Vector copy(src);
        swap(copy);
        return *this;
    }

    void swap(Vector& o) {
        std::swap(storage, o.storage);
        std::swap(length, o.length);
        std::swap(room, o.room);
    }

    void push_back(const T& item) {
        if (length >= room) reallocate(room ? room * 2 : 4);
        storage[length++] = item;
    }

    void push_back(T&& item) {
        if (length >= room) reallocate(room ? room * 2 : 4);
        storage[length++] = std::move(item);
    }

    T& operator[](std::size_t at) { return storage[at]; }
    const T& operator[](std::size_t at) const { return storage[at]; }

    std::size_t size() const { return length; }
    T* data() { return storage; }
    const T* data() const { return storage; }
};

bool contains(const Vector<int>& vec, int target) {
    for (std::size_t i = 0; i < vec.size(); ++i)
        if (vec[i] == target) return true;
    return false;
}

int main() {
    std::size_t n;
    std::cin >> n;
    Vector<int> str;
    for (std::size_t i = 0; i < n; ++i) {
        int x;
        std::cin >> x;
        str.push_back(x);
    }

    Vector<int> filtered;
    for (std::size_t i = 0; i < str.size(); ++i) {
        if (!contains(filtered, str[i]))
            filtered.push_back(str[i]);
    }

    long long total = 0, mult = 1;
    const long long MOD = 1'000'000'007LL;
    for (std::size_t i = 0; i < filtered.size(); ++i) {
        total += filtered[i];
        mult = (mult * filtered[i]) % MOD;
    }

    std::cout << "SUM " << total << '\n';
    std::cout << "PROD " << mult << '\n';
    return 0;
}
