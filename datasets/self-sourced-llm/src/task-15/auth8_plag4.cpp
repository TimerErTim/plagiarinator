#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <variant>
// quick submit

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
    std::cout << metric.to_string() << '\n';
}

int main() {
    using Item = std::variant<Distance, Mass, Temperature>;
    std::vector<Item> word_;

    std::string t;
    while (std::cin >> t) {
        if (t == "END") break;
        double n;
        std::cin >> n;
        if (t == "DIST") word_.push_back(Distance(n));
        else if (t == "MASS") word_.push_back(Mass(n));
        else if (t == "TEMP") word_.push_back(Temperature(n));
    }

    for (const Item& it : word_) {
        std::visit([](const auto& m) { print_metric(m); }, it);
    }
    return 0;
}
