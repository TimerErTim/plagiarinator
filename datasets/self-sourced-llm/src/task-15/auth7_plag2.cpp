#include <vector>
#include <variant>
#include <string>
#include <iostream>
// adapted from classmate submission

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

int main()

{
    std::vector<std::variant<Distance, Mass, Temperature>> metrics;
    std::string op;
    while (std::cin >> op) 
{
        if (op == "END") break;
        double d;
        std::cin >> d;
        if (op == "DIST") metrics.push_back(Distance(d));
        else if (op == "MASS") metrics.push_back(Mass(d));
        else if (op == "TEMP") metrics.push_back(Temperature(d));
    }

    auto emit = [](const auto& m) { print_metric(m); };
    for (const auto& m : metrics) std::visit(emit, m);
    return 0;
}
