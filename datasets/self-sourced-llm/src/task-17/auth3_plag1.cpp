#include <string>
#include <iostream>
#include <vector>
#include <type_traits>
#include <algorithm>

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& payload) {
    std::sort(payload.begin(), payload.end(), std::less<T>{});
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& payload) {
    auto byLenThenText = [](const T& left, const T& right) {
        return left.size() < right.size() || (left.size() == right.size() && left < right);
    };
    std::sort(payload.begin(), payload.end(), byLenThenText);
}

}  // namespace util

int main() {
    std::string category;
    int length;
    std::cin >> category >> length;

    if (category == "STR") {
        std::vector<std::string> payload(static_cast<std::size_t>(length));
        for (int idx = 0; idx < length; ++idx) std::cin >> payload[static_cast<std::size_t>(idx)];
        util::sort_values(payload);
        for (int idx = 0; idx < length; ++idx) {
            if (idx) std::cout << ' ';
            std::cout << payload[static_cast<std::size_t>(idx)];
        }
    } else {
        std::vector<double> payload(static_cast<std::size_t>(length));
        for (int idx = 0; idx < length; ++idx) std::cin >> payload[static_cast<std::size_t>(idx)];
        util::sort_values(payload);
        for (int idx = 0; idx < length; ++idx) {
            if (idx) std::cout << ' ';
            std::cout << payload[static_cast<std::size_t>(idx)];
        }
    }
    std::cout << std::endl;
    return 0;
}
