#include <iostream>
#include <iomanip>
#include <string>
#include <variant>
#include <vector>
#include <cstdio>

template <typename Derived>
class Printable {
public:
    std::string to_string() const {
        return static_cast<const Derived*>(this)->format();
    }
};

struct Distance : Printable<Distance> {
    double value{};
    explicit Distance(double v) : value(v) {}
    std::string format() const {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Distance: %.1f m", value);
        return buf;
    }
};

struct Mass : Printable<Mass> {
    double value{};
    explicit Mass(double v) : value(v) {}
    std::string format() const {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Mass: %.1f kg", value);
        return buf;
    }
};

struct Temperature : Printable<Temperature> {
    double value{};
    explicit Temperature(double v) : value(v) {}
    std::string format() const {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Temp: %.1f C", value);
        return buf;
    }
};

template <typename T>
void print_metric(const Printable<T>& m) {
    std::cout << m.to_string() << std::endl;
}

using Metric = std::variant<Distance, Mass, Temperature>;

static void append_entry(const std::string& tag, double num, std::vector<Metric>& list)
{
    if (tag == "DIST") list.push_back(Distance(num));
    else if (tag == "MASS") list.push_back(Mass(num));
    else if (tag == "TEMP") list.push_back(Temperature(num));
}

int main()
{
    std::vector<Metric> list;
    std::string tag;
    double num;

    for (std::string tok; std::cin >> tok;)
    {
        if (tok == "END") break;
        if (!(std::cin >> num)) break;
        append_entry(tok, num, list);
    }

    for (std::size_t j = 0; j < list.size(); ++j)
    {
        std::visit([](const auto& typed) { print_metric(typed); }, list[j]);
    }
    return 0;
}
