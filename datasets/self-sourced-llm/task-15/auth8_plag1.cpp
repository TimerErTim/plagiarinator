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

static std::string format_val(const char* head, double val, const char* tail) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(1) << head << val << tail;
    return os.str();
}

struct Distance : Printable<Distance> {
    double m{};
    explicit Distance(double v) : m(v) {}
    std::string format() const { return format_val("Distance: ", m, " m"); }
};

struct Mass : Printable<Mass> {
    double m{};
    explicit Mass(double v) : m(v) {}
    std::string format() const { return format_val("Mass: ", m, " kg"); }
};

struct Temperature : Printable<Temperature> {
    double m{};
    explicit Temperature(double v) : m(v) {}
    std::string format() const { return format_val("Temp: ", m, " C"); }
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
