#include <cstdio>
#include <vector>
#include <variant>
#include <string>
#include <iostream>

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
    std::cout << metric.to_string() << std::endl;
}

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> rows;
    std::string tag;
    while (std::cin >> tag) {
        if (tag == "END") break;
        double d;
        std::cin >> d;
        if (tag == "DIST") rows.push_back(Distance(d));
        else if (tag == "MASS") rows.push_back(Mass(d));
        else if (tag == "TEMP") rows.push_back(Temperature(d));
    }

    auto emit = [](const auto& m) { print_metric(m); };
    for (const auto& m : rows) std::visit(emit, m);
    return 0;
}
