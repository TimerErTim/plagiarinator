#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
// TODO: refactor later

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

namespace shapes {

class Circle : public Shape {
    double ref;
public:
    explicit Circle(double rVal) : ref(rVal) {}
    double area() const override {
        static constexpr double pi = 3.141592653589793;
        return pi * ref * ref;
    }
    double perimeter() const override {
        static constexpr double pi = 3.141592653589793;
        return 2.0 * pi * ref;
    }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double argW, argH;
public:
    Rectangle(double w, double h) : argW(w), argH(h) {}
    double area() const override { return argW * argH; }
    double perimeter() const override { return 2.0 * (argW + argH); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double val;
public:
    explicit Triangle(double sVal) : val(sVal) {}
    double area() const override {
        return std::sqrt(3.0) / 4.0 * val * val;
    }
    double perimeter() const override { return 3.0 * val; }
    std::string name() const override { return "Triangle"; }
};

} // namespace shapes

void print_shape(const Shape& s) {
    std::cout << std::fixed << std::setprecision(4)
              << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << '\n';
}

void accumulate(const Shape& s, double& area_sum, double& peri_sum) {
    area_sum += s.area();
    peri_sum += s.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> buf;
    std::string typ;
    while (std::cin >> typ && typ != "DONE") {
        if (typ == "CIRCLE") {
            double r; std::cin >> r;
            buf.push_back(std::make_unique<shapes::Circle>(r));
        } else if (typ == "RECT") {
            double w, h; std::cin >> w >> h;
            buf.push_back(std::make_unique<shapes::Rectangle>(w, h));
        } else if (typ == "TRI") {
            double s; std::cin >> s;
            buf.push_back(std::make_unique<shapes::Triangle>(s));
        }
    }

    double areaTotal = 0.0, periTotal = 0.0;
    for (const auto& fig : buf) {
        print_shape(*fig);
        accumulate(*fig, areaTotal, periTotal);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaTotal << std::endl
              << "TOTAL perimeter=" << periTotal << std::endl;
    return 0;
}
