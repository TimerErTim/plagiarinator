#include <vector>
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <string>

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& buf) {
    std::sort(buf.begin(), buf.end());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& buf) {
    std::sort(buf.begin(), buf.end(), [](const T& first, const T& second) {
        if (first.size() != second.size()) return first.size() < second.size();
        return first < second;
    });
}

}  // namespace util

int main() {
    std::string inputType;
    std::cin >> inputType;
    std::size_t elemCount;
    std::cin >> elemCount;

    if (inputType == "NUM") {
        std::vector<double> values(elemCount);
        for (auto& val : values) std::cin >> val;
        util::sort_values(values);
        for (std::size_t idx = 0; idx < elemCount; ++idx) {
            if (idx) std::cout << ' ';
            std::cout << values[idx];
        }
        std::cout << std::endl;
    } else {
        std::vector<std::string> tokens(elemCount);
        for (auto& token : tokens) std::cin >> token;
        util::sort_values(tokens);
        for (std::size_t idx = 0; idx < elemCount; ++idx) {
            if (idx) std::cout << ' ';
            std::cout << tokens[idx];
        }
        std::cout << std::endl;
    }
    return 0;
}
