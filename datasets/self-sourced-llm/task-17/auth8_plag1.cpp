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
    for (std::size_t i = 0; i < data.size(); ++i)
        for (std::size_t j = i + 1; j < data.size(); ++j)
            if (data[j] < data[i]) std::swap(data[i], data[j]);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    std::sort(data.begin(), data.end(), [](const T& p, const T& q) {
        return (p.size() < q.size()) || (p.size() == q.size() && p < q);
    });
}

}  // namespace util

int main() {
    std::string kind;
    std::size_t n;
    if (!(std::cin >> kind >> n)) return 0;

    if (kind == "STR") {
        std::vector<std::string> tokens(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> tokens[i];
        util::sort_values(tokens);
        for (std::size_t i = 0; i < n; ++i)
            std::cout << (i ? " " : "") << tokens[i];
        std::cout << std::endl;
    } else {
        std::vector<double> nums(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> nums[i];
        util::sort_values(nums);
        for (std::size_t i = 0; i < n; ++i)
            std::cout << (i ? " " : "") << nums[i];
        std::cout << std::endl;
    }
    return 0;
}
