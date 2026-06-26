#include <iomanip>
#include <iostream>
#include <sstream>
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

struct Distance : Printable<Distance> {
    double value_m;
    explicit Distance(double m) : value_m(m) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Distance: " << value_m << " m";
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
    double value_c;
    explicit Temperature(double c) : value_c(c) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Temp: " << value_c << " C";
        return oss.str();
    }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> items;
    std::string kind;
    while (std::cin >> kind && kind != "END") {
        double n;
        std::cin >> n;
        if (kind == "DIST") items.emplace_back(Distance{n});
        else if (kind == "MASS") items.emplace_back(Mass{n});
        else if (kind == "TEMP") items.emplace_back(Temperature{n});
    }

    for (const auto& elem : items) {
        std::visit([](const auto& m) { print_metric(m); }, elem);
    }
    return 0;
}
