#include <vector>
#include <iostream>
#include <iomanip>
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
    double meters;
    Distance(double m) : meters(m) {}
    std::string format() const;
};

struct Mass : Printable<Mass> {
    double kilograms;
    Mass(double k) : kilograms(k) {}
    std::string format() const;
};

struct Temperature : Printable<Temperature> {
    double celsius;
    Temperature(double c) : celsius(c) {}
    std::string format() const;
};

std::string Distance::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Distance: " << meters << " m";
    return ss.str();
}

std::string Mass::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Mass: " << kilograms << " kg";
    return ss.str();
}

std::string Temperature::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Temp: " << celsius << " C";
    return ss.str();
}

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> data;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "END") break;
        double x;
        std::cin >> x;
        if (ln == "DIST") data.emplace_back(Distance{x});
        else if (ln == "MASS") data.emplace_back(Mass{x});
        else if (ln == "TEMP") data.emplace_back(Temperature{x});
    }
    for (auto& v : data)
        std::visit([](auto& m) { print_metric(m); }, v);
    return 0;
}
