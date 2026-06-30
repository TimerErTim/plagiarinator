#include <vector>
#include <utility>
#include <functional>
#include <iostream>
#include <string>
#include <type_traits>
#include <algorithm>
// found online

namespace ranker
{

template <typename Cell>
typename std::enable_if<std::is_arithmetic<Cell>::value, void>::type
sort_values(std::vector<Cell>& lane)
{
    std::sort(lane.begin(), lane.end(), std::less<>());
}

template <typename Cell>
typename std::enable_if<!std::is_arithmetic<Cell>::value, void>::type
sort_values(std::vector<Cell>& lane)
{
    struct SizeThenWord
    {
        bool operator()(const Cell& front, const Cell& back) const
        {
            return std::make_pair(front.size(), front) < std::make_pair(back.size(), back);
        }
    };
    std::sort(lane.begin(), lane.end(), SizeThenWord{});
}

}  // namespace ranker

int main()
{
    std::string marker;
    int span;
    std::cin >> marker >> span;

    if (marker == "NUM")
    {
        std::vector<double> lane(static_cast<std::size_t>(span));
        for (int step = 0; step < span; ++step) std::cin >> lane[static_cast<std::size_t>(step)];
        ranker::sort_values(lane);
        for (int step = 0; step < span; ++step)
        {
            if (step) std::cout << ' ';
            std::cout << lane[static_cast<std::size_t>(step)];
        }
    }
    else
    {
        std::vector<std::string> lane(static_cast<std::size_t>(span));
        for (int step = 0; step < span; ++step) std::cin >> lane[static_cast<std::size_t>(step)];
        ranker::sort_values(lane);
        for (int step = 0; step < span; ++step)
        {
            if (step) std::cout << ' ';
            std::cout << lane[static_cast<std::size_t>(step)];
        }
    }
    std::cout << std::endl;
    return 0;
}
