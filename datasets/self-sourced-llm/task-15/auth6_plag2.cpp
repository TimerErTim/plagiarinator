#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

template <typename Derived>
class Printable {
public:
    std::string to_string() const {
        return static_cast<const Derived*>(this)->format();
    }
};

struct Distance : Printable<Distance> {
    double meters{};
    explicit Distance(double m) : meters(m) {}
    std::string format() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1);
        out << "Distance: " << meters << " m";
        return out.str();
    }
};

struct Mass : Printable<Mass> {
    double kg{};
    explicit Mass(double k) : kg(k) {}
    std::string format() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1);
        out << "Mass: " << kg << " kg";
        return out.str();
    }
};

struct Temperature : Printable<Temperature> {
    double c{};
    explicit Temperature(double t) : c(t) {}
    std::string format() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1);
        out << "Temp: " << c << " C";
        return out.str();
    }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::puts(metric.to_string().c_str());
}

using MetricBox = std::variant<Distance, Mass, Temperature>;

static void parse_and_store(const std::string& line, double val, std::vector<MetricBox>& store)
{
    if (line == "DIST") store.emplace_back(Distance(val));
    else if (line == "MASS") store.emplace_back(Mass(val));
    else if (line == "TEMP") store.emplace_back(Temperature(val));
}

int main()
{
    std::vector<MetricBox> store;
    std::string line;

    while (std::cin >> line)
    {
        if (line == "END") break;
        double val;
        std::cin >> val;
        parse_and_store(line, val, store);
    }

    for (std::size_t p = 0; p < store.size(); ++p)
        std::visit([](const auto& m) { print_metric(m); }, store[p]);
    return 0;
}
