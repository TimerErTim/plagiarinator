#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <variant>
// tweaked from lab partner code

template <typename D>
class HasFormat {
public:
    std::string to_string() const {
        return static_cast<const D*>(this)->format();
    }
};

static std::string fmtNum(double x) {
    char b[32];
    std::snprintf(b, sizeof(b), "%.1f", x);
    return b;
}

struct DistAmt : HasFormat<DistAmt> {
    double amount{};
    explicit DistAmt(double v) : amount(v) {}
    std::string format() const { return "Distance: " + fmtNum(amount) + " m"; }
};

struct MassAmt : HasFormat<MassAmt> {
    double amount{};
    explicit MassAmt(double v) : amount(v) {}
    std::string format() const { return "Mass: " + fmtNum(amount) + " kg"; }
};

struct TempAmt : HasFormat<TempAmt> {
    double amount{};
    explicit TempAmt(double v) : amount(v) {}
    std::string format() const { return "Temp: " + fmtNum(amount) + " C"; }
};

template <typename T>
void put_line(const HasFormat<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    std::vector<std::variant<DistAmt, MassAmt, TempAmt>> metrics;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        double d;
        std::cin >> d;
        if (op == "DIST") metrics.emplace_back(DistAmt(d));
        else if (op == "MASS") metrics.emplace_back(MassAmt(d));
        else if (op == "TEMP") metrics.emplace_back(TempAmt(d));
    }

    for (std::size_t tail = metrics.size(); tail-- > 0;)
        std::visit([](const auto& m) { put_line(m); }, metrics[metrics.size() - 1 - tail]);
    return 0;
}
