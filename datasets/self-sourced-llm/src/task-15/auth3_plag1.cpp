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

namespace metrics {

struct Distance : Printable<Distance> {
    double m{};
    explicit Distance(double v) : m(v) {}
    std::string format() const {
        std::ostringstream o;
        o << std::fixed << std::setprecision(1) << "Distance: " << m << " m";
        return o.str();
    }
};

struct Mass : Printable<Mass> {
    double kg{};
    explicit Mass(double v) : kg(v) {}
    std::string format() const {
        std::ostringstream o;
        o << std::fixed << std::setprecision(1) << "Mass: " << kg << " kg";
        return o.str();
    }
};

struct Temperature : Printable<Temperature> {
    double c{};
    explicit Temperature(double v) : c(v) {}
    std::string format() const {
        std::ostringstream o;
        o << std::fixed << std::setprecision(1) << "Temp: " << c << " C";
        return o.str();
    }
};

}  // namespace metrics

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

using Record = std::variant<metrics::Distance, metrics::Mass, metrics::Temperature>;

struct RecordVisitor {
    template <typename M>
    void operator()(const M& m) const { print_metric(m); }
};

int main() {
    std::vector<Record> rows;
    for (std::string type; std::cin >> type && type != "END";) {
        double n;
        std::cin >> n;
        if (type == "DIST") rows.emplace_back(metrics::Distance(n));
        else if (type == "MASS") rows.emplace_back(metrics::Mass(n));
        else if (type == "TEMP") rows.emplace_back(metrics::Temperature(n));
    }

    for (const auto& rhs : rows) std::visit(RecordVisitor{}, rhs);
    return 0;
}
