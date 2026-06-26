#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
// cleaned up version

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
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return kPi * radius_ * radius_; }
    double perimeter() const override { return 2.0 * kPi * radius_; }
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
    double edge_;
public:
    explicit Triangle(double e) : edge_(e) {}
    double area() const override {
        return std::sqrt(3.0) * edge_ * edge_ / 4.0;
    }
    double perimeter() const override { return 3.0 * edge_; }
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
    std::vector<std::unique_ptr<Shape>> items;
    std::string elem;
    while (std::cin >> elem) {
        if (elem == "DONE") break;
        if (elem == "CIRCLE") {
            double r; std::cin >> r;
            items.emplace_back(std::make_unique<Circle>(r));
        } else if (elem == "RECT") {
            double w, h; std::cin >> w >> h;
            items.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (elem == "TRI") {
            double s; std::cin >> s;
            items.emplace_back(std::make_unique<Triangle>(s));
        }
    }

    double areaSum = 0.0, periSum = 0.0;
    for (const std::unique_ptr<Shape>& ptr : items) {
        print_shape(*ptr);
        accumulate(*ptr, areaSum, periSum);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaSum << std::endl
              << "TOTAL perimeter=" << periSum << std::endl;
    return 0;
}
