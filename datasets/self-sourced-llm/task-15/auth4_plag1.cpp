#include <iomanip>
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

struct Distance : Printable<Distance> {
    double value_m;
    Distance(double m) : value_m(m) {}
    std::string format() const;
};

struct Mass : Printable<Mass> {
    double value_kg;
    Mass(double k) : value_kg(k) {}
    std::string format() const;
};

struct Temperature : Printable<Temperature> {
    double value_c;
    Temperature(double c) : value_c(c) {}
    std::string format() const;
};

std::string Distance::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Distance: " << value_m << " m";
    return ss.str();
}

std::string Mass::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Mass: " << value_kg << " kg";
    return ss.str();
}

std::string Temperature::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Temp: " << value_c << " C";
    return ss.str();
}

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> data;
    std::string piece;
    while (std::cin >> piece) {
        if (piece == "END") break;
        double lhs;
        std::cin >> lhs;
        if (piece == "DIST") data.emplace_back(Distance{lhs});
        else if (piece == "MASS") data.emplace_back(Mass{lhs});
        else if (piece == "TEMP") data.emplace_back(Temperature{lhs});
    }
    for (auto& n : data)
        std::visit([](auto& m) { print_metric(m); }, n);
    return 0;
}
