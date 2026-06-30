#include <string>
#include <vector>
#include <iostream>
#include <type_traits>
#include <algorithm>

namespace util {

template <typename T, typename = void>
struct sort_impl;

template <typename T>
struct sort_impl<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    static void apply(std::vector<T>& heap) { std::sort(heap.begin(), heap.end()); }
};

template <typename T>
struct sort_impl<T, typename std::enable_if<!std::is_arithmetic<T>::value>::type> {
    static void apply(std::vector<T>& heap) {
        std::sort(heap.begin(), heap.end(), [](const T& left, const T& right) {
            if (left.size() != right.size()) return left.size() < right.size();
            return left < right;
        });
    }
};

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& heap) {
    sort_impl<T>::apply(heap);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& heap) {
    sort_impl<T>::apply(heap);
}

}  // namespace util

int main() {
    std::string selector;
    int span;
    std::cin >> selector >> span;

    if (selector == "NUM") {
        std::vector<double> heap(static_cast<std::size_t>(span));
        for (int k = 0; k < span; ++k) std::cin >> heap[static_cast<std::size_t>(k)];
        util::sort_values(heap);
        for (int k = 0; k < span; ++k) {
            if (k) std::cout << ' ';
            std::cout << heap[static_cast<std::size_t>(k)];
        }
    } else {
        std::vector<std::string> heap(static_cast<std::size_t>(span));
        for (int k = 0; k < span; ++k) std::cin >> heap[static_cast<std::size_t>(k)];
        util::sort_values(heap);
        for (int k = 0; k < span; ++k) {
            if (k) std::cout << ' ';
            std::cout << heap[static_cast<std::size_t>(k)];
        }
    }
    std::cout << std::endl;
    return 0;
}
