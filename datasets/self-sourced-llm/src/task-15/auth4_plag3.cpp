#include <sstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <variant>
// tweaked from lab partner code

template <typename Concrete>
class TextBase {
public:
    std::string to_string() const {
        return static_cast<const Concrete*>(this)->format();
    }
};

struct LenMeasure : TextBase<LenMeasure> {
    double meters;
    LenMeasure(double m) : meters(m) {}
    std::string format() const;
};

struct WtMeasure : TextBase<WtMeasure> {
    double kilograms;
    WtMeasure(double k) : kilograms(k) {}
    std::string format() const;
};

struct CMeasure : TextBase<CMeasure> {
    double celsius;
    CMeasure(double c) : celsius(c) {}
    std::string format() const;
};

std::string LenMeasure::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Distance: " << meters << " m";
    return ss.str();
}

std::string WtMeasure::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Mass: " << kilograms << " kg";
    return ss.str();
}

std::string CMeasure::format() const {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1) << "Temp: " << celsius << " C";
    return ss.str();
}

template <typename T>
void write_out(const TextBase<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<LenMeasure, WtMeasure, CMeasure>> data;
    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        double x;
        std::cin >> x;
        if (cmd == "DIST") data.emplace_back(LenMeasure{x});
        else if (cmd == "MASS") data.emplace_back(WtMeasure{x});
        else if (cmd == "TEMP") data.emplace_back(CMeasure{x});
    }
    for (std::size_t step = data.size(); step-- > 0;)
        std::visit([](const auto& m) { write_out(m); }, data[data.size() - 1 - step]);
    return 0;
}
