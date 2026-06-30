#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <variant>
// tweaked from lab partner code

template <typename Node>
class OutText {
public:
    std::string to_string() const {
        return static_cast<const Node*>(this)->format();
    }
};

struct DistNode : OutText<DistNode> {
    double meters{};
    explicit DistNode(double m) : meters(m) {}
    std::string format() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1);
        out << "Distance: " << meters << " m";
        return out.str();
    }
};

struct MassNode : OutText<MassNode> {
    double kg{};
    explicit MassNode(double k) : kg(k) {}
    std::string format() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1);
        out << "Mass: " << kg << " kg";
        return out.str();
    }
};

struct TempNode : OutText<TempNode> {
    double c{};
    explicit TempNode(double t) : c(t) {}
    std::string format() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1);
        out << "Temp: " << c << " C";
        return out.str();
    }
};

template <typename T>
void flush_row(const OutText<T>& metric) {
    std::puts(metric.to_string().c_str());
}

int main() {
    std::vector<std::variant<DistNode, MassNode, TempNode>> store;
    std::string line;

    while (std::cin >> line) {
        if (line == "END") break;
        double val;
        std::cin >> val;
        if (line == "DIST") store.emplace_back(DistNode(val));
        else if (line == "MASS") store.emplace_back(MassNode(val));
        else if (line == "TEMP") store.emplace_back(TempNode(val));
    }

    for (std::size_t r = store.size(); r-- > 0;)
        std::visit([](const auto& m) { flush_row(m); }, store[store.size() - 1 - r]);
    return 0;
}
