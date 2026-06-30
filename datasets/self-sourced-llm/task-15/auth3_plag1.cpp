#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

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
    std::cout << metric.to_string() << std::endl;
}

using Record = std::variant<metrics::Distance, metrics::Mass, metrics::Temperature>;

struct RecordVisitor {
    template <typename M>
    void operator()(const M& m) const { print_metric(m); }
};

int main() {
    std::vector<Record> entries;
    for (std::string tag; std::cin >> tag && tag != "END";) {
        double v;
        std::cin >> v;
        if (tag == "DIST") entries.push_back(metrics::Distance(v));
        else if (tag == "MASS") entries.push_back(metrics::Mass(v));
        else if (tag == "TEMP") entries.push_back(metrics::Temperature(v));
    }

    for (const auto& r : entries) std::visit(RecordVisitor{}, r);
    return 0;
}
