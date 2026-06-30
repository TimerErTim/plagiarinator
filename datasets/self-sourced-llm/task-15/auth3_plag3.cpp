#include <variant>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
// tweaked from lab partner code

template <typename Impl>
class AsText {
public:
    std::string to_string() const {
        return static_cast<const Impl*>(this)->format();
    }
};

namespace readings {

struct DistVal : AsText<DistVal> {
    double m{};
    explicit DistVal(double v) : m(v) {}
    std::string format() const {
        std::ostringstream o;
        o << std::fixed << std::setprecision(1) << "Distance: " << m << " m";
        return o.str();
    }
};

struct MassVal : AsText<MassVal> {
    double kg{};
    explicit MassVal(double v) : kg(v) {}
    std::string format() const {
        std::ostringstream o;
        o << std::fixed << std::setprecision(1) << "Mass: " << kg << " kg";
        return o.str();
    }
};

struct TempVal : AsText<TempVal> {
    double c{};
    explicit TempVal(double v) : c(v) {}
    std::string format() const {
        std::ostringstream o;
        o << std::fixed << std::setprecision(1) << "Temp: " << c << " C";
        return o.str();
    }
};

}  // namespace readings

template <typename T>
void dump_row(const AsText<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

using Row = std::variant<readings::DistVal, readings::MassVal, readings::TempVal>;

struct DumpVisitor {
    template <typename M>
    void operator()(const M& m) const { dump_row(m); }
};

int main() {
    std::vector<Row> rows;
    for (std::string type; std::cin >> type && type != "END";) {
        double v;
        std::cin >> v;
        if (type == "DIST") rows.emplace_back(readings::DistVal(v));
        else if (type == "MASS") rows.emplace_back(readings::MassVal(v));
        else if (type == "TEMP") rows.emplace_back(readings::TempVal(v));
    }

    for (std::size_t back = rows.size(); back-- > 0;)
        std::visit(DumpVisitor{}, rows[rows.size() - 1 - back]);
    return 0;
}
