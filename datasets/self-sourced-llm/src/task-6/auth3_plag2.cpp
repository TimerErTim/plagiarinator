#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>
// adapted from classmate submission

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
    double rad_;
public:
    explicit Circle(double r) : rad_(r) {}
    double area() const override { return kPi * rad_ * rad_; }
    double perimeter() const override { return 2.0 * kPi * rad_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    double area() const override { return width_ * height_; }
    double perimeter() const override { return 2.0 * (width_ + height_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double side_;
public:
    explicit Triangle(double e) : side_(e) {}
    double area() const override {
        return std::sqrt(3.0) * side_ * side_ / 4.0;
    }
    double perimeter() const override { return 3.0 * side_; }
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

int main()
{
    std::vector<std::unique_ptr<Shape>> gallery;
    std::string op;
    while (std::cin >> op)
{
        if (op == "DONE") break;
        if (op == "CIRCLE")
{
            double r; std::cin >> r;
            gallery.emplace_back(std::make_unique<Circle>(r));
        } else if (op == "RECT") {
            double w, h; std::cin >> w >> h;
            gallery.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (op == "TRI") {
            double s; std::cin >> s;
            gallery.emplace_back(std::make_unique<Triangle>(s));
        }
    }

    double a = 0.0, p = 0.0;
    for (const std::unique_ptr<Shape>& shapePtr : gallery)
{
        print_shape(*shapePtr);
        accumulate(*shapePtr, a, p);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << a << std::endl
              << "TOTAL perimeter=" << p << std::endl;
    return 0;
}
