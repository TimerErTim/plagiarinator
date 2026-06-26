#include <vector>
#include <variant>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
// adapted from classmate submission

template <typename Derived>
class Printable {
public:
    std::string to_string() const {
        return static_cast<const Derived*>(this)->format();
    }
};

static std::string make_str(const char* head, double val, const char* tail) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(1) << head << val << tail;
    return os.str();
}

struct Distance : Printable<Distance> {
    double m{};
    explicit Distance(double v) : m(v) {}
    std::string format() const { return make_str("Distance: ", m, " m"); }
};

struct Mass : Printable<Mass> {
    double m{};
    explicit Mass(double v) : m(v) {}
    std::string format() const { return make_str("Mass: ", m, " kg"); }
};

struct Temperature : Printable<Temperature> {
    double m{};
    explicit Temperature(double v) : m(v) {}
    std::string format() const { return make_str("Temp: ", m, " C"); }
};

template <typename T>
void print_metric(const Printable<T>& metric) {
    std::cout << metric.to_string() << '\n';
}

int main()

{
    using Item = std::variant<Distance, Mass, Temperature>;
    std::vector<Item> items;

    std::string t;
    while (std::cin >> t) 
{
        if (t == "END") break;
        double n;
        std::cin >> n;
        if (t == "DIST") items.push_back(Distance(n));
        else if (t == "MASS") items.push_back(Mass(n));
        else if (t == "TEMP") items.push_back(Temperature(n));
    }

    for (const Item& it : items) 
{
        std::visit([](const auto& m) { print_metric(m); }, it);
    }
    return 0;
}
