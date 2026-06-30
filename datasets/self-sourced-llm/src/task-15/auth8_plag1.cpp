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

static std::string render(const char* head, double val, const char* tail) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(1) << head << val << tail;
    return os.str();
}

struct Distance : Printable<Distance> {
    double m{};
    explicit Distance(double v) : m(v) {}
    std::string format() const { return render("Distance: ", m, " m"); }
};

struct Mass : Printable<Mass> {
    double m{};
    explicit Mass(double v) : m(v) {}
    std::string format() const { return render("Mass: ", m, " kg"); }
};

struct Temperature : Printable<Temperature> {
    double m{};
    explicit Temperature(double v) : m(v) {}
    std::string format() const { return render("Temp: ", m, " C"); }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << std::endl;
}

int main() {
    using Item = std::variant<Distance, Mass, Temperature>;
    std::vector<Item> list;

    std::string tag;
    while (std::cin >> tag) {
        if (tag == "END") break;
        double n;
        std::cin >> n;
        if (tag == "DIST") list.emplace_back(Distance(n));
        else if (tag == "MASS") list.emplace_back(Mass(n));
        else if (tag == "TEMP") list.emplace_back(Temperature(n));
    }

    for (const Item& it : list) {
        std::visit([](const auto& m) { print_metric(m); }, it);
    }
    return 0;
}
