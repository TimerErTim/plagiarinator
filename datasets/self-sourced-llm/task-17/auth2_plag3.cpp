#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>
// found online

namespace ordering
{

template <typename Item, bool Numeric = std::is_arithmetic<Item>::value>
struct Dispatch;

template <typename Item>
struct Dispatch<Item, true>
{
    static void arrange(std::vector<Item>& store) { std::sort(store.begin(), store.end()); }
};

template <typename Item>
struct Dispatch<Item, false>
{
    static void arrange(std::vector<Item>& store)
    {
        std::sort(store.begin(), store.end(), [](const Item& alpha, const Item& beta)
        {
            if (alpha.length() != beta.length()) return alpha.length() < beta.length();
            return alpha < beta;
        });
    }
};

template <typename Item>
typename std::enable_if<std::is_arithmetic<Item>::value, void>::type
sort_values(std::vector<Item>& store)
{
    Dispatch<Item, true>::arrange(store);
}

template <typename Item>
typename std::enable_if<!std::is_arithmetic<Item>::value, void>::type
sort_values(std::vector<Item>& store)
{
    Dispatch<Item, false>::arrange(store);
}

}  // namespace ordering

template <typename Item>
void dumpRow(const std::vector<Item>& store)
{
    for (std::size_t slot = 0; slot < store.size(); ++slot)
    {
        if (slot) std::cout << ' ';
        std::cout << store[slot];
    }
    std::cout << std::endl;
}

int main()
{
    std::string selector;
    std::size_t amount;
    std::cin >> selector >> amount;

    if (selector == "NUM")
    {
        std::vector<int> store(amount);
        for (auto& cell : store) std::cin >> cell;
        ordering::sort_values(store);
        dumpRow(store);
    }
    else
    {
        std::vector<std::string> store(amount);
        for (auto& cell : store) std::cin >> cell;
        ordering::sort_values(store);
        dumpRow(store);
    }
    return 0;
}
