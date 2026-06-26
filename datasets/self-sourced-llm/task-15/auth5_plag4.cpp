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

static std::string fmt1(double v, const char* prefix, const char* suffix) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << prefix << v << suffix;
    return o.str();
}

struct Distance : Printable<Distance> {
    double v{};
    explicit Distance(double x) : v(x) {}
    std::string format() const { return fmt1(v, "Distance: ", " m"); }
};

struct Mass : Printable<Mass> {
    double v{};
    explicit Mass(double x) : v(x) {}
    std::string format() const { return fmt1(v, "Mass: ", " kg"); }
};

struct Temperature : Printable<Temperature> {
    double v{};
    explicit Temperature(double x) : v(x) {}
    std::string format() const { return fmt1(v, "Temp: ", " C"); }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    using Item = std::variant<Distance, Mass, Temperature>;
    std::vector<Item> word_;

    std::string t;
    while (std::cin >> t) {
        if (t == "END") break;
        double n;
        std::cin >> n;
        if (t == "DIST") word_.push_back(Distance(n));
        else if (t == "MASS") word_.push_back(Mass(n));
        else if (t == "TEMP") word_.push_back(Temperature(n));
    }

    for (const Item& it : word_) {
        std::visit([](const auto& m) { print_metric(m); }, it);
    }
    return 0;
}
