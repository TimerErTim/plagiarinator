#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

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

using Item = std::variant<Distance, Mass, Temperature>;

static void add_item(const std::string& t, double n, std::vector<Item>& items)
{
    if (t == "DIST") items.push_back(Distance(n));
    else if (t == "MASS") items.push_back(Mass(n));
    else if (t == "TEMP") items.push_back(Temperature(n));
}

int main()
{
    std::vector<Item> items;

    for (std::string t; std::cin >> t;)
    {
        if (t == "END") break;
        double n;
        std::cin >> n;
        add_item(t, n, items);
    }

    for (std::size_t k = 0; k < items.size(); ++k)
        std::visit([](const auto& m) { print_metric(m); }, items[k]);
    return 0;
}
