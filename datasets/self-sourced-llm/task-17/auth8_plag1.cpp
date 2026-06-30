#include <vector>
#include <type_traits>
#include <iostream>
#include <string>
#include <algorithm>

namespace util {

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& batch) {
    for (std::size_t outer = 0; outer < batch.size(); ++outer)
        for (std::size_t inner = outer + 1; inner < batch.size(); ++inner)
            if (batch[inner] < batch[outer]) std::swap(batch[outer], batch[inner]);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& batch) {
    std::sort(batch.begin(), batch.end(), [](const T& head, const T& tail) {
        return (head.size() < tail.size()) || (head.size() == tail.size() && head < tail);
    });
}

}  // namespace util

int main() {
    std::string kind;
    std::size_t length;
    if (!(std::cin >> kind >> length)) return 0;

    if (kind == "STR") {
        std::vector<std::string> batch(length);
        for (auto& token : batch) std::cin >> token;
        util::sort_values(batch);
        for (std::size_t slot = 0; slot < length; ++slot)
            std::cout << (slot ? " " : "") << batch[slot];
        std::cout << std::endl;
    } else {
        std::vector<double> batch(length);
        for (auto& token : batch) std::cin >> token;
        util::sort_values(batch);
        for (std::size_t slot = 0; slot < length; ++slot)
            std::cout << (slot ? " " : "") << batch[slot];
        std::cout << std::endl;
    }
    return 0;
}
