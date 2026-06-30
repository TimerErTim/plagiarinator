#include <cstdio>
#include <iostream>
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

static std::string one_decimal(double x) {
    char b[32];
    std::snprintf(b, sizeof(b), "%.1f", x);
    return b;
}

struct Distance : Printable<Distance> {
    double amount{};
    explicit Distance(double v) : amount(v) {}
    std::string format() const { return "Distance: " + one_decimal(amount) + " m"; }
};

struct Mass : Printable<Mass> {
    double amount{};
    explicit Mass(double v) : amount(v) {}
    std::string format() const { return "Mass: " + one_decimal(amount) + " kg"; }
};

struct Temperature : Printable<Temperature> {
    double amount{};
    explicit Temperature(double v) : amount(v) {}
    std::string format() const { return "Temp: " + one_decimal(amount) + " C"; }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

using MetricVar = std::variant<Distance, Mass, Temperature>;

static void read_metric(const std::string& op, double d, std::vector<MetricVar>& metrics)
{
    if (op == "DIST") metrics.emplace_back(Distance(d));
    else if (op == "MASS") metrics.emplace_back(Mass(d));
    else if (op == "TEMP") metrics.emplace_back(Temperature(d));
}

int main()
{
    std::vector<MetricVar> metrics;
    std::string op;
    while (std::cin >> op)
    {
        if (op == "END") break;
        double d;
        std::cin >> d;
        read_metric(op, d, metrics);
    }

    for (std::size_t idx = 0; idx < metrics.size(); ++idx)
        std::visit([](const auto& m) { print_metric(m); }, metrics[idx]);
    return 0;
}
