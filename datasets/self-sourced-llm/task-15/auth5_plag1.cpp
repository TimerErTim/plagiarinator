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

static std::string format_metric(double v, const char* prefix, const char* suffix) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << prefix << v << suffix;
    return o.str();
}

struct Distance : Printable<Distance> {
    double v{};
    explicit Distance(double lhs) : v(lhs) {}
    std::string format() const { return format_metric(v, "Distance: ", " m"); }
};

struct Mass : Printable<Mass> {
    double v{};
    explicit Mass(double lhs) : v(lhs) {}
    std::string format() const { return format_metric(v, "Mass: ", " kg"); }
};

struct Temperature : Printable<Temperature> {
    double v{};
    explicit Temperature(double lhs) : v(lhs) {}
    std::string format() const { return format_metric(v, "Temp: ", " C"); }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    using Item = std::variant<Distance, Mass, Temperature>;
    std::vector<Item> items;

    std::string piece;
    while (std::cin >> piece) {
        if (piece == "END") break;
        double cnt;
        std::cin >> cnt;
        if (piece == "DIST") items.push_back(Distance(cnt));
        else if (piece == "MASS") items.push_back(Mass(cnt));
        else if (piece == "TEMP") items.push_back(Temperature(cnt));
    }

    for (const Item& it : items) {
        std::visit([](const auto& m) { print_metric(m); }, it);
    }
    return 0;
}
