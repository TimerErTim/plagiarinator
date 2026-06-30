#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <type_traits>
// found online

namespace pack
{

template <typename Unit>
typename std::enable_if<std::is_arithmetic<Unit>::value, void>::type
sort_values(std::vector<Unit>& tray)
{
    std::sort(tray.begin(), tray.end());
}

template <typename Unit>
typename std::enable_if<!std::is_arithmetic<Unit>::value, void>::type
sort_values(std::vector<Unit>& tray)
{
    std::sort(tray.begin(), tray.end(), [](const Unit& u, const Unit& v)
    {
        if (u.size() < v.size()) return true;
        if (v.size() < u.size()) return false;
        return u < v;
    });
}

}  // namespace pack

void flushDoubles(std::vector<double>& tray)
{
    pack::sort_values(tray);
    for (std::size_t offset = 0; offset < tray.size(); ++offset)
    {
        if (offset) std::cout << ' ';
        std::cout << tray[offset];
    }
    std::cout << std::endl;
}

void flushStrings(std::vector<std::string>& tray)
{
    pack::sort_values(tray);
    for (std::size_t offset = 0; offset < tray.size(); ++offset)
    {
        if (offset) std::cout << ' ';
        std::cout << tray[offset];
    }
    std::cout << std::endl;
}

int main()
{
    std::string gate;
    std::size_t cap;
    std::cin >> gate >> cap;
    if (gate == "NUM")
    {
        std::vector<double> tray(cap);
        for (std::size_t offset = 0; offset < cap; ++offset) std::cin >> tray[offset];
        flushDoubles(tray);
    }
    else
    {
        std::vector<std::string> tray(cap);
        for (std::size_t offset = 0; offset < cap; ++offset) std::cin >> tray[offset];
        flushStrings(tray);
    }
    return 0;
}
