#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

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
    std::cout << metric.to_string() << std::endl;
}

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> entries;
    std::string label;
    while (std::cin >> label && label != "END") {
        double num;
        std::cin >> num;
        if (label == "DIST") entries.push_back(Distance{num});
        else if (label == "MASS") entries.push_back(Mass{num});
        else if (label == "TEMP") entries.push_back(Temperature{num});
    }

    for (const auto& row : entries) {
        std::visit([](const auto& m) { print_metric(m); }, row);
    }
    return 0;
}
