#include <utility>
#include <functional>
#include <type_traits>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& record) {
    std::sort(record.begin(), record.end(), std::less<>());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& record) {
    struct LenLex {
        bool operator()(const T& u, const T& v) const {
            return std::make_pair(u.size(), u) < std::make_pair(v.size(), v);
        }
    };
    std::sort(record.begin(), record.end(), LenLex{});
}

}  // namespace util

int main() {
    std::string label;
    int amount;
    std::cin >> label >> amount;

    if (label == "NUM") {
        std::vector<double> record(static_cast<std::size_t>(amount));
        for (int slot = 0; slot < amount; ++slot) std::cin >> record[static_cast<std::size_t>(slot)];
        util::sort_values(record);
        for (int slot = 0; slot < amount; ++slot) {
            if (slot) std::cout << ' ';
            std::cout << record[static_cast<std::size_t>(slot)];
        }
    } else {
        std::vector<std::string> record(static_cast<std::size_t>(amount));
        for (int slot = 0; slot < amount; ++slot) std::cin >> record[static_cast<std::size_t>(slot)];
        util::sort_values(record);
        for (int slot = 0; slot < amount; ++slot) {
            if (slot) std::cout << ' ';
            std::cout << record[static_cast<std::size_t>(slot)];
        }
    }
    std::cout << std::endl;
    return 0;
}
