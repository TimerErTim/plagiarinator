#include <vector>
#include <type_traits>
#include <iostream>
#include <string>
#include <algorithm>

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& store);

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& store);

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& store) {
    std::sort(store.begin(), store.end());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& store) {
    std::sort(store.begin(), store.end(), [](const T& one, const T& two) {
        if (one.length() != two.length()) return one.length() < two.length();
        return one < two;
    });
}

}  // namespace util

template <typename Container>
void dump(const Container& block) {
    bool leading = true;
    for (const auto& piece : block) {
        if (!leading) std::cout << ' ';
        leading = false;
        std::cout << piece;
    }
    std::cout << std::endl;
}

int main() {
    std::string flavor;
    std::size_t total;
    std::cin >> flavor >> total;

    if (flavor == "STR") {
        std::vector<std::string> store(total);
        for (std::size_t idx = 0; idx < total; ++idx) std::cin >> store[idx];
        util::sort_values(store);
        dump(store);
    } else {
        std::vector<double> store(total);
        for (std::size_t idx = 0; idx < total; ++idx) std::cin >> store[idx];
        util::sort_values(store);
        dump(store);
    }
    return 0;
}
