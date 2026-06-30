#include <cstdio>
#include <vector>
#include <variant>
#include <string>
#include <iostream>
#include <iomanip>

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
    std::cout << m.to_string() << '\n';
}

using Metric = std::variant<Distance, Mass, Temperature>;

int main() {
    std::vector<Metric> rows;
    std::string label;
    double num;

    while (std::cin >> label) {
        if (label == "END") break;
        if (!(std::cin >> num)) break;
        if (label == "DIST") rows.emplace_back(Distance(num));
        else if (label == "MASS") rows.emplace_back(Mass(num));
        else if (label == "TEMP") rows.emplace_back(Temperature(num));
    }

    for (const Metric& entry : rows) {
        std::visit([](const auto& typed) { print_metric(typed); }, entry);
    }
    return 0;
}
