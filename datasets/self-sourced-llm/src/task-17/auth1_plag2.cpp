#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace util
{

template <typename T>
bool lengthThenLex(const T& left, const T& right)
{
    if (left.size() != right.size()) return left.size() < right.size();
    return left < right;
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    std::sort(data.begin(), data.end());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    std::sort(data.begin(), data.end(), lengthThenLex<T>);
}

}  // namespace util

template <typename T>
void emitRow(const std::vector<T>& row)
{
    for (std::size_t i = 0; i < row.size(); ++i)
    {
        if (i) std::cout << ' ';
        std::cout << row[i];
    }
    std::cout << '\n';
}

int main()
{
    std::string mode;
    std::cin >> mode;
    std::size_t n;
    std::cin >> n;

    if (mode == "NUM")
    {
        std::vector<double> nums(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> nums[i];
        util::sort_values(nums);
        emitRow(nums);
    }
    else
    {
        std::vector<std::string> words(n);
        for (std::size_t i = 0; i < n; ++i) std::cin >> words[i];
        util::sort_values(words);
        emitRow(words);
    }
    return 0;
}
