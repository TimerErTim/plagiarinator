#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    std::sort(data.begin(), data.end());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    std::sort(data.begin(), data.end(), [](const T& a, const T& b) {
        if (a.size() != b.size()) return a.size() < b.size();
        return a < b;
    });
}

}  // namespace util

int main() {
    std::string mode;
    std::cin >> mode;
    std::size_t n;
    std::cin >> n;

    if (mode == "NUM") {
        std::vector<double> nums(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> nums[i];
        util::sort_values(nums);
        for (std::size_t i = 0; i < n; ++i) {
            if (i) std::cout << ' ';
            std::cout << nums[i];
        }
        std::cout << '\n';
    } else {
        std::vector<std::string> words(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> words[i];
        util::sort_values(words);
        for (std::size_t i = 0; i < n; ++i) {
            if (i) std::cout << ' ';
            std::cout << words[i];
        }
        std::cout << '\n';
    }
    return 0;
}
