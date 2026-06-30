#include <variant>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
// tweaked from lab partner code

template <typename Self>
class Formattable {
public:
    std::string to_string() const {
        return static_cast<const Self*>(this)->format();
    }
};

struct DistReading : Formattable<DistReading> {
    double meters;
    explicit DistReading(double m) : meters(m) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Distance: " << meters << " m";
        return oss.str();
    }
};

struct WeightReading : Formattable<WeightReading> {
    double kg;
    explicit WeightReading(double k) : kg(k) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Mass: " << kg << " kg";
        return oss.str();
    }
};

struct HeatReading : Formattable<HeatReading> {
    double celsius;
    explicit HeatReading(double c) : celsius(c) {}
    std::string format() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "Temp: " << celsius << " C";
        return oss.str();
    }
};

template <typename T>
void emit_line(const Formattable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<DistReading, WeightReading, HeatReading>> records;
    std::string kind;
    while (std::cin >> kind && kind != "END") {
        double val;
        std::cin >> val;
        if (kind == "DIST") records.emplace_back(DistReading{val});
        else if (kind == "MASS") records.emplace_back(WeightReading{val});
        else if (kind == "TEMP") records.emplace_back(HeatReading{val});
    }

    for (std::size_t rev = records.size(); rev-- > 0;) {
        std::visit([](const auto& m) { emit_line(m); }, records[records.size() - 1 - rev]);
    }
    return 0;
}
