#include <variant>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
// tweaked from lab partner code

template <typename Unit>
class ToLine {
public:
    std::string to_string() const {
        return static_cast<const Unit*>(this)->format();
    }
};

static std::string mkLine(const char* head, double val, const char* tail) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(1) << head << val << tail;
    return os.str();
}

struct DistPack : ToLine<DistPack> {
    double m{};
    explicit DistPack(double v) : m(v) {}
    std::string format() const { return mkLine("Distance: ", m, " m"); }
};

struct MassPack : ToLine<MassPack> {
    double m{};
    explicit MassPack(double v) : m(v) {}
    std::string format() const { return mkLine("Mass: ", m, " kg"); }
};

struct TempPack : ToLine<TempPack> {
    double m{};
    explicit TempPack(double v) : m(v) {}
    std::string format() const { return mkLine("Temp: ", m, " C"); }
};

template <typename T>
void emit_pack(const ToLine<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main() {
    using Item = std::variant<DistPack, MassPack, TempPack>;
    std::vector<Item> items;

    std::string t;
    while (std::cin >> t) {
        if (t == "END") break;
        double n;
        std::cin >> n;
        if (t == "DIST") items.push_back(DistPack(n));
        else if (t == "MASS") items.push_back(MassPack(n));
        else if (t == "TEMP") items.push_back(TempPack(n));
    }

    for (std::size_t rem = items.size(); rem-- > 0;)
        std::visit([](const auto& m) { emit_pack(m); }, items[items.size() - 1 - rem]);
    return 0;
}
