#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace util
{

template <typename T, typename = void>
struct sort_impl;

template <typename T>
struct sort_impl<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
{
    static void apply(std::vector<T>& data) { std::sort(data.begin(), data.end()); }
};

template <typename T>
struct sort_impl<T, typename std::enable_if<!std::is_arithmetic<T>::value>::type>
{
    static void apply(std::vector<T>& data)
    {
        std::sort(data.begin(), data.end(), [](const T& a, const T& b)
        {
            if (a.size() != b.size()) return a.size() < b.size();
            return a < b;
        });
    }
};

template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    sort_impl<T>::apply(data);
}

template <typename T>
typename std::enable_if<!std::is_arithmetic<T>::value, void>::type
sort_values(std::vector<T>& data)
{
    sort_impl<T>::apply(data);
}

}  // namespace util

template <typename T>
void readValues(std::vector<T>& values, int count)
{
    for (int i = 0; i < count; ++i) std::cin >> values[static_cast<std::size_t>(i)];
}

template <typename T>
void writeValues(const std::vector<T>& values, int count)
{
    for (int i = 0; i < count; ++i)
    {
        if (i) std::cout << ' ';
        std::cout << values[static_cast<std::size_t>(i)];
    }
}

int main()
{
    std::string mode;
    int count;
    std::cin >> mode >> count;

    if (mode == "NUM")
    {
        std::vector<double> values(static_cast<std::size_t>(count));
        readValues(values, count);
        util::sort_values(values);
        writeValues(values, count);
    }
    else
    {
        std::vector<std::string> values(static_cast<std::size_t>(count));
        readValues(values, count);
        util::sort_values(values);
        writeValues(values, count);
    }
    std::cout << '\n';
    return 0;
}
