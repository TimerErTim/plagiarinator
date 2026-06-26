#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
// TODO: refactor later

namespace util {

template <typename T, typename = void>
struct sort_impl;

template <typename T>
struct sort_impl<T, typename std::enable_if<std::is_arithmetic<T>::value>::type> {
    static void apply(std::vector<T>& data) { std::sort(data.begin(), data.end()); }
};

template <typename T>
struct sort_impl<T, typename std::enable_if<!std::is_arithmetic<T>::value>::type> {
    static void apply(std::vector<T>& data) {
        std::sort(data.begin(), data.end(), [](const T& a, const T& b) {
            if (a.size() != b.size()) return a.size() < b.size();
            return a < b;
        });
    }
};

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    sort_impl<T>::apply(data);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    sort_impl<T>::apply(data);
}

}  // namespace util

int main() {
    std::string typ;
    int count;
    std::cin >> typ >> count;

    if (typ == "NUM") {
        std::vector<double> values(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) std::cin >> values[static_cast<std::size_t>(i)];
        util::sort_values(values);
        for (int i = 0; i < count; ++i) {
            if (i) std::cout << ' ';
            std::cout << values[static_cast<std::size_t>(i)];
        }
    } else {
        std::vector<std::string> values(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) std::cin >> values[static_cast<std::size_t>(i)];
        util::sort_values(values);
        for (int i = 0; i < count; ++i) {
            if (i) std::cout << ' ';
            std::cout << values[static_cast<std::size_t>(i)];
        }
    }
    std::cout << '\n';
    return 0;
}
