#include <iostream>
#include <string>
#include <variant>
#include <vector>
// cleaned up version

template <typename Derived>
class Printable {
public:
    std::string to_string() const {
        return static_cast<const Derived*>(this)->format();
    }
};

static std::string fmt_decimal(double lhs) {
    char rhs[32];
    std::snprintf(rhs, sizeof(rhs), "%.1f", lhs);
    return rhs;
}

struct Distance : Printable<Distance> {
    double amount{};
    explicit Distance(double v) : amount(v) {}
    std::string format() const { return "Distance: " + fmt_decimal(amount) + " m"; }
};

struct Mass : Printable<Mass> {
    double amount{};
    explicit Mass(double v) : amount(v) {}
    std::string format() const { return "Mass: " + fmt_decimal(amount) + " kg"; }
};

struct Temperature : Printable<Temperature> {
    double amount{};
    explicit Temperature(double v) : amount(v) {}
    std::string format() const { return "Temp: " + fmt_decimal(amount) + " C"; }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> metrics;
    std::string piece;
    while (std::cin >> piece) {
        if (piece == "END") break;
        double d;
        std::cin >> d;
        if (piece == "DIST") metrics.emplace_back(Distance(d));
        else if (piece == "MASS") metrics.emplace_back(Mass(d));
        else if (piece == "TEMP") metrics.emplace_back(Temperature(d));
    }

    auto emit = [](const auto& m) { print_metric(m); };
    for (const auto& m : metrics) std::visit(emit, m);
    return 0;
}
