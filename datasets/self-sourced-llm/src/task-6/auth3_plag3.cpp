#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
// TODO: refactor later

namespace {
constexpr double kPi = 3.141592653589793;
}

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double ref;
public:
    explicit Circle(double r) : ref(r) {}
    double area() const override { return kPi * ref * ref; }
    double perimeter() const override { return 2.0 * kPi * ref; }
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
    explicit Triangle(double e) : val(e) {}
    double area() const override {
        return std::sqrt(3.0) * val * val / 4.0;
    }
    double perimeter() const override { return 3.0 * val; }
    std::string name() const override { return "Triangle"; }
};

static void print_shape(const Shape& figure) {
    std::cout << std::fixed << std::setprecision(4)
              << figure.name() << ": area=" << figure.area()
              << ", perimeter=" << figure.perimeter() << std::endl;
}

static void accumulate(const Shape& figure, double& area_sum, double& peri_sum) {
    area_sum += figure.area();
    peri_sum += figure.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> gallery;
    std::string ln;
    while (std::cin >> ln) {
        if (ln == "DONE") break;
        if (ln == "CIRCLE") {
            double r; std::cin >> r;
            gallery.emplace_back(std::make_unique<Circle>(r));
        } else if (ln == "RECT") {
            double w, h; std::cin >> w >> h;
            gallery.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (ln == "TRI") {
            double s; std::cin >> s;
            gallery.emplace_back(std::make_unique<Triangle>(s));
        }
    }

    double areaTotal = 0.0, periTotal = 0.0;
    for (const std::unique_ptr<Shape>& shapePtr : gallery) {
        print_shape(*shapePtr);
        accumulate(*shapePtr, areaTotal, periTotal);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaTotal << std::endl
              << "TOTAL perimeter=" << periTotal << std::endl;
    return 0;
}
