#include <vector>
#include <type_traits>
#include <algorithm>
#include <iostream>
#include <string>
// found online

namespace arrange_ns
{

template <typename Entry>
typename std::enable_if<std::is_arithmetic<Entry>::value, void>::type
reorder(std::vector<Entry>& heap)
{
    std::sort(heap.begin(), heap.end(), std::less<Entry>{});
}

template <typename Entry>
typename std::enable_if<!std::is_arithmetic<Entry>::value, void>::type
reorder(std::vector<Entry>& heap)
{
    auto ranker = [](const Entry& one, const Entry& two)
    {
        return one.size() < two.size() || (one.size() == two.size() && one < two);
    };
    std::sort(heap.begin(), heap.end(), ranker);
}

}  // namespace arrange_ns

int main()
{
    std::string flavor;
    int tally;
    std::cin >> flavor >> tally;

    if (flavor == "STR")
    {
        std::vector<std::string> heap(static_cast<std::size_t>(tally));
        for (int cursor = 0; cursor < tally; ++cursor)
            std::cin >> heap[static_cast<std::size_t>(cursor)];
        arrange_ns::reorder(heap);
        for (int cursor = 0; cursor < tally; ++cursor)
        {
            if (cursor) std::cout << ' ';
            std::cout << heap[static_cast<std::size_t>(cursor)];
        }
    }
    else
    {
        std::vector<double> heap(static_cast<std::size_t>(tally));
        for (int cursor = 0; cursor < tally; ++cursor)
            std::cin >> heap[static_cast<std::size_t>(cursor)];
        arrange_ns::reorder(heap);
        for (int cursor = 0; cursor < tally; ++cursor)
        {
            if (cursor) std::cout << ' ';
            std::cout << heap[static_cast<std::size_t>(cursor)];
        }
    }
    std::cout << std::endl;
    return 0;
}
