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
    std::cout << m.to_string() << std::endl;
}

using Metric = std::variant<Distance, Mass, Temperature>;

int main() {
    std::vector<Metric> list;
    std::string tag;
    double n;

    while (std::cin >> tag) {
        if (tag == "END") break;
        if (!(std::cin >> n)) break;
        if (tag == "DIST") list.push_back(Distance(n));
        else if (tag == "MASS") list.push_back(Mass(n));
        else if (tag == "TEMP") list.push_back(Temperature(n));
    }

    for (const Metric& entry : list) {
        std::visit([](const auto& typed) { print_metric(typed); }, entry);
    }
    return 0;
}
