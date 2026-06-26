#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
// cleaned up version

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    std::sort(data.begin(), data.end(), std::less<>());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    struct LenLex {
        bool operator()(const T& u, const T& v) const {
            return std::make_pair(u.size(), u) < std::make_pair(v.size(), v);
        }
    };
    std::sort(data.begin(), data.end(), LenLex{});
}

}  // namespace util

int main() {
    std::string label;
    int qty;
    std::cin >> label >> qty;

    if (label == "NUM") {
        std::vector<double> s(static_cast<std::size_t>(qty));
        for (int i = 0; i < qty; ++i) std::cin >> s[static_cast<std::size_t>(i)];
        util::sort_values(s);
        for (int i = 0; i < qty; ++i) {
            if (i) std::cout << ' ';
            std::cout << s[static_cast<std::size_t>(i)];
        }
    } else {
        std::vector<std::string> s(static_cast<std::size_t>(qty));
        for (int i = 0; i < qty; ++i) std::cin >> s[static_cast<std::size_t>(i)];
        util::sort_values(s);
        for (int i = 0; i < qty; ++i) {
            if (i) std::cout << ' ';
            std::cout << s[static_cast<std::size_t>(i)];
        }
    }
    std::cout << std::endl;
    return 0;
}
