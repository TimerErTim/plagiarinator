#include <iomanip>
#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
// quick submit

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
    double word_;
public:
    explicit Circle(double r) : word_(r) {}
    double area() const override { return kPi * word_ * word_; }
    double perimeter() const override { return 2.0 * kPi * word_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double elemW_, elemH_;
public:
    Rectangle(double w, double h) : elemW_(w), elemH_(h) {}
    double area() const override { return elemW_ * elemH_; }
    double perimeter() const override { return 2.0 * (elemW_ + elemH_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double tmp_;
public:
    explicit Triangle(double e) : tmp_(e) {}
    double area() const override {
        return std::sqrt(3.0) * tmp_ * tmp_ / 4.0;
    }
    double perimeter() const override { return 3.0 * tmp_; }
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
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "DONE") break;
        if (elem == "CIRCLE") {
            double r; std::cin >> r;
            gallery.emplace_back(std::make_unique<Circle>(r));
        } else if (elem == "RECT") {
            double w, h; std::cin >> w >> h;
            gallery.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (elem == "TRI") {
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
