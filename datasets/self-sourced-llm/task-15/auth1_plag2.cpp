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
    double meters;
    explicit Distance(double m) : meters(m) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Distance: " << meters << " m";
        return oss.str();
    }
};

struct Mass : Printable<Mass> {
    double kg;
    explicit Mass(double k) : kg(k) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Mass: " << kg << " kg";
        return oss.str();
    }
};

struct Temperature : Printable<Temperature> {
    double celsius;
    explicit Temperature(double c) : celsius(c) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Temp: " << celsius << " C";
        return oss.str();
    }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

using MetricVar = std::variant<Distance, Mass, Temperature>;

static void store_metric(const std::string& kind, double val, std::vector<MetricVar>& out)
{
    if (kind == "DIST") out.emplace_back(Distance{val});
    else if (kind == "MASS") out.emplace_back(Mass{val});
    else if (kind == "TEMP") out.emplace_back(Temperature{val});
}

int main()
{
    std::vector<MetricVar> records;
    std::string kind;
    while (std::cin >> kind && kind != "END")
    {
        double val;
        std::cin >> val;
        store_metric(kind, val, records);
    }

    for (std::size_t idx = 0; idx < records.size(); ++idx)
    {
        std::visit([](const auto& m) { print_metric(m); }, records[idx]);
    }
    return 0;
}
