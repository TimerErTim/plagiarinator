#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace util
{

template <typename T>
void bubbleSortArithmetic(std::vector<T>& data)
{
    for (std::size_t i = 0; i < data.size(); ++i)
        for (std::size_t j = i + 1; j < data.size(); ++j)
            if (data[j] < data[i]) std::swap(data[i], data[j]);
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    bubbleSortArithmetic(data);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    std::sort(data.begin(), data.end(), [](const T& p, const T& q)
    {
        return (p.size() < q.size()) || (p.size() == q.size() && p < q);
    });
}

}  // namespace util

template <typename T>
void readAndPrint(std::size_t n)
{
    std::vector<T> tokens(n);
    for (std::size_t i = 0; i < n; ++i) std::cin >> tokens[i];
    util::sort_values(tokens);
    for (std::size_t i = 0; i < n; ++i)
        std::cout << (i ? " " : "") << tokens[i];
    std::cout << '\n';
}

int main()
{
    std::string mode;
    std::size_t n;
    if (!(std::cin >> mode >> n)) return 0;

    if (mode == "STR")
        readAndPrint<std::string>(n);
    else
        readAndPrint<double>(n);
    return 0;
}
