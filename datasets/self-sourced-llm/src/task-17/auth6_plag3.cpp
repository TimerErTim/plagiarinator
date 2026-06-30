#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <algorithm>
// found online

namespace pipeline
{

template <typename Chunk>
typename std::enable_if<std::is_arithmetic<Chunk>::value, void>::type
sort_values(std::vector<Chunk>& rack);

template <typename Chunk>
typename std::enable_if<!std::is_arithmetic<Chunk>::value, void>::type
sort_values(std::vector<Chunk>& rack);

template <typename Chunk>
typename std::enable_if<std::is_arithmetic<Chunk>::value, void>::type
sort_values(std::vector<Chunk>& rack)
{
    std::sort(rack.begin(), rack.end());
}

template <typename Chunk>
typename std::enable_if<!std::is_arithmetic<Chunk>::value, void>::type
sort_values(std::vector<Chunk>& rack)
{
    std::sort(rack.begin(), rack.end(), [](const Chunk& alpha, const Chunk& beta)
    {
        if (alpha.length() != beta.length()) return alpha.length() < beta.length();
        return alpha < beta;
    });
}

}  // namespace pipeline

template <typename Rack>
void spill(const Rack& rack)
{
    bool gap = true;
    for (const auto& unit : rack)
    {
        if (!gap) std::cout << ' ';
        gap = false;
        std::cout << unit;
    }
    std::cout << std::endl;
}

int main()
{
    std::string channel;
    std::size_t width;
    std::cin >> channel >> width;

    if (channel == "STR")
    {
        std::vector<std::string> rack(width);
        for (std::size_t pin = 0; pin < width; ++pin) std::cin >> rack[pin];
        pipeline::sort_values(rack);
        spill(rack);
    }
    else
    {
        std::vector<double> rack(width);
        for (std::size_t pin = 0; pin < width; ++pin) std::cin >> rack[pin];
        pipeline::sort_values(rack);
        spill(rack);
    }
    return 0;
}
