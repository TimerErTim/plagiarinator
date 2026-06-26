#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <variant>
// TODO: refactor later

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

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> store;
    std::string buf;

    while (std::cin >> buf) {
        if (buf == "END") break;
        double val;
        std::cin >> val;
        if (buf == "DIST") store.emplace_back(Distance(val));
        else if (buf == "MASS") store.emplace_back(Mass(val));
        else if (buf == "TEMP") store.emplace_back(Temperature(val));
    }

    for (const auto& rec : store) {
        std::visit([](const auto& m) { print_metric(m); }, rec);
    }
    return 0;
}
