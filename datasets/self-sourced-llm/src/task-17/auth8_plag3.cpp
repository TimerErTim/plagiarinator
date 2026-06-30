#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <type_traits>
// found online

namespace mutate
{

template <typename Piece>
void naiveNumericSort(std::vector<Piece>& chain)
{
    for (std::size_t head = 0; head < chain.size(); ++head)
        for (std::size_t tail = head + 1; tail < chain.size(); ++tail)
            if (chain[tail] < chain[head]) std::swap(chain[head], chain[tail]);
}

template <typename Piece>
typename std::enable_if<std::is_arithmetic<Piece>::value, void>::type
sort_values(std::vector<Piece>& chain)
{
    naiveNumericSort(chain);
}

template <typename Piece>
typename std::enable_if<!std::is_arithmetic<Piece>::value, void>::type
sort_values(std::vector<Piece>& chain)
{
    std::sort(chain.begin(), chain.end(), [](const Piece& low, const Piece& high)
    {
        return (low.size() < high.size()) || (low.size() == high.size() && low < high);
    });
}

}  // namespace mutate

int main()
{
    std::string channel;
    std::size_t volume;
    if (!(std::cin >> channel >> volume)) return 0;

    if (channel == "STR")
    {
        std::vector<std::string> chain(volume);
        for (std::size_t lane = 0; lane < volume; ++lane) std::cin >> chain[lane];
        mutate::sort_values(chain);
        for (std::size_t lane = 0; lane < volume; ++lane)
            std::cout << (lane ? " " : "") << chain[lane];
        std::cout << std::endl;
    }
    else
    {
        std::vector<double> chain(volume);
        for (std::size_t lane = 0; lane < volume; ++lane) std::cin >> chain[lane];
        mutate::sort_values(chain);
        for (std::size_t lane = 0; lane < volume; ++lane)
            std::cout << (lane ? " " : "") << chain[lane];
        std::cout << std::endl;
    }
    return 0;
}
