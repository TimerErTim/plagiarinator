#include <string>
#include <type_traits>
#include <vector>
#include <iostream>
#include <algorithm>

namespace util {

template <typename T, bool IsArithmetic = std::is_arithmetic<T>::value>
struct Sorter;

template <typename T>
struct Sorter<T, true> {
    static void run(std::vector<T>& payload) { std::sort(payload.begin(), payload.end()); }
};

template <typename T>
struct Sorter<T, false> {
    static void run(std::vector<T>& payload) {
        std::sort(payload.begin(), payload.end(), [](const T& left, const T& right) {
            if (left.length() != right.length()) return left.length() < right.length();
            return left < right;
        });
    }
};

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& payload) {
    Sorter<T, true>::run(payload);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& payload) {
    Sorter<T, false>::run(payload);
}

}  // namespace util

template <typename T>
void showLine(const std::vector<T>& seq) {
    for (std::size_t k = 0; k < seq.size(); ++k) {
        if (k) std::cout << ' ';
        std::cout << seq[k];
    }
    std::cout << std::endl;
}

int main() {
    std::string opMode;
    std::size_t total;
    std::cin >> opMode >> total;

    if (opMode == "NUM") {
        std::vector<int> numbers(total);
        for (int i = 0; i < static_cast<int>(total); ++i) std::cin >> numbers[static_cast<std::size_t>(i)];
        util::sort_values(numbers);
        showLine(numbers);
    } else {
        std::vector<std::string> labels(total);
        for (int i = 0; i < static_cast<int>(total); ++i) std::cin >> labels[static_cast<std::size_t>(i)];
        util::sort_values(labels);
        showLine(labels);
    }
    return 0;
}
