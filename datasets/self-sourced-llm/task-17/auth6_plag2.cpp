#include <vector>
#include <type_traits>
#include <string>
#include <iostream>
#include <algorithm>
// adapted from classmate submission

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data);

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data);

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    std::sort(data.begin(), data.end());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data) {
    std::sort(data.begin(), data.end(), [](const T& s1, const T& s2) {
        if (s1.length() != s2.length()) return s1.length() < s2.length();
        return s1 < s2;
    });
}

}  // namespace util

template <typename Container>
void dump(const Container& c) {
    bool first = true;
    for (const auto& item : c) {
        if (!first) std::cout << ' ';
        first = false;
        std::cout << item;
    }
    std::cout << '\n';
}

int main()
{
    std::string tag;
    std::size_t n;
    std::cin >> tag >> n;

    if (tag == "STR") {
        std::vector<std::string> data(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> data[i];
        util::sort_values(data);
        dump(data);
    } else {
        std::vector<double> data(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> data[i];
        util::sort_values(data);
        dump(data);
    }
    return 0;
}
