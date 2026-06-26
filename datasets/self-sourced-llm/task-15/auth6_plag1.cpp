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
    double value_m{};
    explicit Distance(double m) : value_m(m) {}
    std::string format() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1);
        out << "Distance: " << value_m << " m";
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

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> store;
    std::string s;

    while (std::cin >> s) {
        if (s == "END") break;
        double n;
        std::cin >> n;
        if (s == "DIST") store.emplace_back(Distance(n));
        else if (s == "MASS") store.emplace_back(Mass(n));
        else if (s == "TEMP") store.emplace_back(Temperature(n));
    }

    for (const auto& rec : store) {
        std::visit([](const auto& m) { print_metric(m); }, rec);
    }
    return 0;
}
