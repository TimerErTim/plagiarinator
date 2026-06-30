#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <variant>
// tweaked from lab partner code

template <typename Child>
class Stringify {
public:
    std::string to_string() const {
        return static_cast<const Child*>(this)->format();
    }
};

struct DistUnit : Stringify<DistUnit> {
    double value{};
    explicit DistUnit(double v) : value(v) {}
    std::string format() const {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Distance: %.1f m", value);
        return buf;
    }
};

struct MassUnit : Stringify<MassUnit> {
    double value{};
    explicit MassUnit(double v) : value(v) {}
    std::string format() const {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Mass: %.1f kg", value);
        return buf;
    }
};

struct TempUnit : Stringify<TempUnit> {
    double value{};
    explicit TempUnit(double v) : value(v) {}
    std::string format() const {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "Temp: %.1f C", value);
        return buf;
    }
};

template <typename T>
void show_reading(const Stringify<T>& m) {
    std::cout << m.to_string() << std::endl;
}

using Reading = std::variant<DistUnit, MassUnit, TempUnit>;

int main() {
    std::vector<Reading> list;
    std::string tag;
    double num;

    while (std::cin >> tag) {
        if (tag == "END") break;
        if (!(std::cin >> num)) break;
        if (tag == "DIST") list.push_back(DistUnit(num));
        else if (tag == "MASS") list.push_back(MassUnit(num));
        else if (tag == "TEMP") list.push_back(TempUnit(num));
    }

    for (std::size_t tail = list.size(); tail-- > 0;) {
        std::visit([](const auto& typed) { show_reading(typed); }, list[list.size() - 1 - tail]);
    }
    return 0;
}
