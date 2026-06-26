#include <vector>
#include <type_traits>
#include <string>
#include <iostream>
#include <functional>
#include <algorithm>
// adapted from classmate submission

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

int main()
{
    std::string tag;
    int qty;
    std::cin >> tag >> qty;

    if (tag == "NUM") {
        std::vector<double> input(static_cast<std::size_t>(qty));
        for (int i = 0; i < qty; ++i) std::cin >> input[static_cast<std::size_t>(i)];
        util::sort_values(input);
        for (int i = 0; i < qty; ++i) {
            if (i) std::cout << ' ';
            std::cout << input[static_cast<std::size_t>(i)];
        }
    } else {
        std::vector<std::string> input(static_cast<std::size_t>(qty));
        for (int i = 0; i < qty; ++i) std::cin >> input[static_cast<std::size_t>(i)];
        util::sort_values(input);
        for (int i = 0; i < qty; ++i) {
            if (i) std::cout << ' ';
            std::cout << input[static_cast<std::size_t>(i)];
        }
    }
    std::cout << '\n';
    return 0;
}
