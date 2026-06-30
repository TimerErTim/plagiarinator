#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <type_traits>
// found online

namespace engine
{

template <typename Node, typename = void>
struct sorter_core;

template <typename Node>
struct sorter_core<Node, typename std::enable_if<std::is_arithmetic<Node>::value>::type>
{
    static void drive(std::vector<Node>& deck) { std::sort(deck.begin(), deck.end()); }
};

template <typename Node>
struct sorter_core<Node, typename std::enable_if<!std::is_arithmetic<Node>::value>::type>
{
    static void drive(std::vector<Node>& deck)
    {
        std::sort(deck.begin(), deck.end(), [](const Node& x, const Node& y)
        {
            if (x.size() != y.size()) return x.size() < y.size();
            return x < y;
        });
    }
};

template <typename Node>
typename std::enable_if<std::is_arithmetic<Node>::value, void>::type
sort_values(std::vector<Node>& deck)
{
    sorter_core<Node>::drive(deck);
}

template <typename Node>
typename std::enable_if<!std::is_arithmetic<Node>::value, void>::type
sort_values(std::vector<Node>& deck)
{
    sorter_core<Node>::drive(deck);
}

}  // namespace engine

int main()
{
    std::string opcode;
    int limit;
    std::cin >> opcode >> limit;

    if (opcode == "NUM")
    {
        std::vector<double> deck(static_cast<std::size_t>(limit));
        for (int tick = 0; tick < limit; ++tick) std::cin >> deck[static_cast<std::size_t>(tick)];
        engine::sort_values(deck);
        for (int tick = 0; tick < limit; ++tick)
        {
            if (tick) std::cout << ' ';
            std::cout << deck[static_cast<std::size_t>(tick)];
        }
    }
    else
    {
        std::vector<std::string> deck(static_cast<std::size_t>(limit));
        for (int tick = 0; tick < limit; ++tick) std::cin >> deck[static_cast<std::size_t>(tick)];
        engine::sort_values(deck);
        for (int tick = 0; tick < limit; ++tick)
        {
            if (tick) std::cout << ' ';
            std::cout << deck[static_cast<std::size_t>(tick)];
        }
    }
    std::cout << std::endl;
    return 0;
}
