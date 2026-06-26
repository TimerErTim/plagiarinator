#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <variant>
// quick submit

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
    std::vector<std::variant<Distance, Mass, Temperature>> records;
    std::string kind;
    while (std::cin >> kind && kind != "END") {
        double val;
        std::cin >> val;
        if (kind == "DIST") records.emplace_back(Distance{val});
        else if (kind == "MASS") records.emplace_back(Mass{val});
        else if (kind == "TEMP") records.emplace_back(Temperature{val});
    }

    for (const auto& tmp : records) {
        std::visit([](const auto& m) { print_metric(m); }, tmp);
    }
    return 0;
}
