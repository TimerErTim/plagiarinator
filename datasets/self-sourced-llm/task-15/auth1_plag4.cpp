#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

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
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<Distance, Mass, Temperature>> list;
    std::string token;
    while (std::cin >> token && token != "END") {
        double num_val;
        std::cin >> num_val;
        if (token == "DIST") list.emplace_back(Distance{num_val});
        else if (token == "MASS") list.emplace_back(Mass{num_val});
        else if (token == "TEMP") list.emplace_back(Temperature{num_val});
    }

    for (const auto& item : list) {
        std::visit([](const auto& m) { print_metric(m); }, item);
    }
    return 0;
}
