#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace util
{

template <typename T>
bool lenLexCompare(const T& lhs, const T& rhs)
{
    return lhs.size() < rhs.size() || (lhs.size() == rhs.size() && lhs < rhs);
}

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    std::sort(data.begin(), data.end(), std::less<T>{});
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    std::sort(data.begin(), data.end(), lenLexCompare<T>);
}

}  // namespace util

template <typename T>
void readItems(std::vector<T>& items, int n)
{
    for (int i = 0; i < n; ++i) std::cin >> items[static_cast<std::size_t>(i)];
}

template <typename T>
void writeItems(const std::vector<T>& items, int n)
{
    for (int i = 0; i < n; ++i)
    {
        if (i) std::cout << ' ';
        std::cout << items[static_cast<std::size_t>(i)];
    }
}

int main()
{
    std::string kind;
    int n;
    std::cin >> kind >> n;

    if (kind == "STR")
    {
        std::vector<std::string> items(static_cast<std::size_t>(n));
        readItems(items, n);
        util::sort_values(items);
        writeItems(items, n);
    }
    else
    {
        std::vector<double> items(static_cast<std::size_t>(n));
        readItems(items, n);
        util::sort_values(items);
        writeItems(items, n);
    }
    std::cout << '\n';
    return 0;
}
