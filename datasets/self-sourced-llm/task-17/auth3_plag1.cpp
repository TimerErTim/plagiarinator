#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
// cleaned up version

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    std::sort(data.begin(), data.end(), std::less<T>{});
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    auto cmp = [](const T& lhs, const T& rhs) {
        return lhs.size() < rhs.size() || (lhs.size() == rhs.size() && lhs < rhs);
    };
    std::sort(data.begin(), data.end(), cmp);
}

}  // namespace util

int main() {
    std::string kind;
    int n;
    std::cin >> kind >> n;

    if (kind == "STR") {
        std::vector<std::string> items(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) std::cin >> items[static_cast<std::size_t>(i)];
        util::sort_values(items);
        for (int i = 0; i < n; ++i) {
            if (i) std::cout << ' ';
            std::cout << items[static_cast<std::size_t>(i)];
        }
    } else {
        std::vector<double> items(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) std::cin >> items[static_cast<std::size_t>(i)];
        util::sort_values(items);
        for (int i = 0; i < n; ++i) {
            if (i) std::cout << ' ';
            std::cout << items[static_cast<std::size_t>(i)];
        }
    }
    std::cout << std::endl;
    return 0;
}
