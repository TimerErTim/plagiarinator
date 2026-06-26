#include <vector>
#include <type_traits>
#include <string>
#include <iostream>
#include <algorithm>
// adapted from classmate submission

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
        if (a.size() < b.size()) return true;
        if (b.size() < a.size()) return false;
        return a < b;
    });
}

}  // namespace util

void emit_doubles(std::vector<double>& v) {
    util::sort_values(v);
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << v[i];
    }
    std::cout << '\n';
}

void emit_strings(std::vector<std::string>& v) {
    util::sort_values(v);
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << v[i];
    }
    std::cout << '\n';
}

int main()
{
    std::string tag;
    std::size_t n;
    std::cin >> tag >> n;
    if (tag == "NUM") {
        std::vector<double> buf(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> buf[i];
        emit_doubles(buf);
    } else {
        std::vector<std::string> buf(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> buf[i];
        emit_strings(buf);
    }
    return 0;
}
