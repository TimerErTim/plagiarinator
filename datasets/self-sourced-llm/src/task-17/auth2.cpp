#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace util {

template <typename T, bool IsArithmetic = std::is_arithmetic<T>::value>
struct Sorter;

template <typename T>
struct Sorter<T, true> {
    static void run(std::vector<T>& data) { std::sort(data.begin(), data.end()); }
};

template <typename T>
struct Sorter<T, false> {
    static void run(std::vector<T>& data) {
        std::sort(data.begin(), data.end(), [](const T& x, const T& y) {
            if (x.length() != y.length()) return x.length() < y.length();
            return x < y;
        });
    }
};

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    Sorter<T, true>::run(data);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    Sorter<T, false>::run(data);
}

}  // namespace util

template <typename T>
void print_vec(const std::vector<T>& v) {
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << v[i];
    }
    std::cout << '\n';
}

int main() {
    std::string mode;
    std::size_t count;
    std::cin >> mode >> count;

    if (mode == "NUM") {
        std::vector<int> vals(count);
        for (auto& x : vals) std::cin >> x;
        util::sort_values(vals);
        print_vec(vals);
    } else {
        std::vector<std::string> vals(count);
        for (auto& s : vals) std::cin >> s;
        util::sort_values(vals);
        print_vec(vals);
    }
    return 0;
}
