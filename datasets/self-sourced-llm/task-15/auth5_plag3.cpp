#include <variant>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
// tweaked from lab partner code

template <typename Leaf>
class LineMaker {
public:
    std::string to_string() const {
        return static_cast<const Leaf*>(this)->format();
    }
};

static std::string buildStr(double v, const char* prefix, const char* suffix) {
    std::ostringstream o;
    o << std::fixed << std::setprecision(1) << prefix << v << suffix;
    return o.str();
}

struct DistLine : LineMaker<DistLine> {
    double v{};
    explicit DistLine(double x) : v(x) {}
    std::string format() const { return buildStr(v, "Distance: ", " m"); }
};

struct MassLine : LineMaker<MassLine> {
    double v{};
    explicit MassLine(double x) : v(x) {}
    std::string format() const { return buildStr(v, "Mass: ", " kg"); }
};

struct TempLine : LineMaker<TempLine> {
    double v{};
    explicit TempLine(double x) : v(x) {}
    std::string format() const { return buildStr(v, "Temp: ", " C"); }
};

template <typename T>
void show_metric(const LineMaker<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    using Item = std::variant<DistLine, MassLine, TempLine>;
    std::vector<Item> items;

    std::string t;
    while (std::cin >> t) {
        if (t == "END") break;
        double n;
        std::cin >> n;
        if (t == "DIST") items.push_back(DistLine(n));
        else if (t == "MASS") items.push_back(MassLine(n));
        else if (t == "TEMP") items.push_back(TempLine(n));
    }

    for (std::size_t left = items.size(); left-- > 0;)
        std::visit([](const auto& m) { show_metric(m); }, items[items.size() - 1 - left]);
    return 0;
}
