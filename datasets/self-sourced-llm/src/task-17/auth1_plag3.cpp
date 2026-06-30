#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
// found online

namespace sorter_ns
{

template <typename Elem>
typename std::enable_if<std::is_arithmetic<Elem>::value, void>::type
order_inplace(std::vector<Elem>& bucket)
{
    std::sort(bucket.begin(), bucket.end());
}

template <typename Elem>
typename std::enable_if<!std::is_arithmetic<Elem>::value, void>::type
order_inplace(std::vector<Elem>& bucket)
{
    std::sort(bucket.begin(), bucket.end(), [](const Elem& lhs, const Elem& rhs)
    {
        if (lhs.size() != rhs.size()) return lhs.size() < rhs.size();
        return lhs < rhs;
    });
}

}  // namespace sorter_ns

int main()
{
    std::string cmd;
    std::cin >> cmd;
    std::size_t qty;
    std::cin >> qty;

    if (cmd == "NUM")
    {
        std::vector<double> numericBucket(qty);
        for (std::size_t pos = 0; pos < qty; ++pos) std::cin >> numericBucket[pos];
        sorter_ns::order_inplace(numericBucket);
        for (std::size_t pos = 0; pos < qty; ++pos)
        {
            if (pos) std::cout << ' ';
            std::cout << numericBucket[pos];
        }
        std::cout << std::endl;
    }
    else
    {
        std::vector<std::string> textBucket(qty);
        for (std::size_t pos = 0; pos < qty; ++pos) std::cin >> textBucket[pos];
        sorter_ns::order_inplace(textBucket);
        for (std::size_t pos = 0; pos < qty; ++pos)
        {
            if (pos) std::cout << ' ';
            std::cout << textBucket[pos];
        }
        std::cout << std::endl;
    }
    return 0;
}
