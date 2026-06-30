#include <iomanip>
#include <iostream>
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

using Entry = std::variant<Distance, Mass, Temperature>;

static void ingest(const std::string& cmd, double x, std::vector<Entry>& data)
{
    if (cmd == "DIST") data.emplace_back(Distance{x});
    else if (cmd == "MASS") data.emplace_back(Mass{x});
    else if (cmd == "TEMP") data.emplace_back(Temperature{x});
}

int main()
{
    std::vector<Entry> data;
    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "END") break;
        double x;
        std::cin >> x;
        ingest(cmd, x, data);
    }
    for (std::size_t k = 0; k < data.size(); ++k)
        std::visit([](const auto& m) { print_metric(m); }, data[k]);
    return 0;
}
