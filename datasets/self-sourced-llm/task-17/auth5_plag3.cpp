#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
// TODO: refactor later

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
    std::string tag;
    int qty;
    std::cin >> tag >> qty;

    if (tag == "NUM") {
        std::vector<double> buf(static_cast<std::size_t>(qty));
        for (int i = 0; i < qty; ++i) std::cin >> buf[static_cast<std::size_t>(i)];
        util::sort_values(buf);
        for (int i = 0; i < qty; ++i) {
            if (i) std::cout << ' ';
            std::cout << buf[static_cast<std::size_t>(i)];
        }
    } else {
        std::vector<std::string> buf(static_cast<std::size_t>(qty));
        for (int i = 0; i < qty; ++i) std::cin >> buf[static_cast<std::size_t>(i)];
        util::sort_values(buf);
        for (int i = 0; i < qty; ++i) {
            if (i) std::cout << ' ';
            std::cout << buf[static_cast<std::size_t>(i)];
        }
    }
    std::cout << '\n';
    return 0;
}
