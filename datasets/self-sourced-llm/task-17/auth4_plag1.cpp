#include <vector>
#include <string>
#include <iostream>
#include <type_traits>
#include <algorithm>

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& bucket) {
    std::sort(bucket.begin(), bucket.end());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& bucket) {
    std::sort(bucket.begin(), bucket.end(), [](const T& left, const T& right) {
        if (left.size() < right.size()) return true;
        if (right.size() < left.size()) return false;
        return left < right;
    });
}

}  // namespace util

void emit_doubles(std::vector<double>& seq) {
    util::sort_values(seq);
    for (std::size_t pos = 0; pos < seq.size(); ++pos) {
        if (pos) std::cout << ' ';
        std::cout << seq[pos];
    }
    std::cout << std::endl;
}

void emit_strings(std::vector<std::string>& seq) {
    util::sort_values(seq);
    for (std::size_t pos = 0; pos < seq.size(); ++pos) {
        if (pos) std::cout << ' ';
        std::cout << seq[pos];
    }
    std::cout << std::endl;
}

int main() {
    std::string selector;
    std::size_t qty;
    std::cin >> selector >> qty;
    if (selector == "NUM") {
        std::vector<double> seq(qty);
        for (auto& entry : seq) std::cin >> entry;
        emit_doubles(seq);
    } else {
        std::vector<std::string> seq(qty);
        for (auto& entry : seq) std::cin >> entry;
        emit_strings(seq);
    }
    return 0;
}
