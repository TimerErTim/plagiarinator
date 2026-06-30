#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace util
{

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    std::sort(data.begin(), data.end(), std::less<>());
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    struct LenLex
    {
        bool operator()(const T& u, const T& v) const
        {
            return std::make_pair(u.size(), u) < std::make_pair(v.size(), v);
        }
    };
    std::sort(data.begin(), data.end(), LenLex{});
}

}  // namespace util

template <typename T>
void readBlock(std::vector<T>& row, int qty)
{
    for (int i = 0; i < qty; ++i) std::cin >> row[static_cast<std::size_t>(i)];
}

template <typename T>
void writeBlock(const std::vector<T>& row, int qty)
{
    for (int i = 0; i < qty; ++i)
    {
        if (i) std::cout << ' ';
        std::cout << row[static_cast<std::size_t>(i)];
    }
}

int main()
{
    std::string tag;
    int qty;
    std::cin >> tag >> qty;

    if (tag == "NUM")
    {
        std::vector<double> row(static_cast<std::size_t>(qty));
        readBlock(row, qty);
        util::sort_values(row);
        writeBlock(row, qty);
    }
    else
    {
        std::vector<std::string> row(static_cast<std::size_t>(qty));
        readBlock(row, qty);
        util::sort_values(row);
        writeBlock(row, qty);
    }
    std::cout << '\n';
    return 0;
}
