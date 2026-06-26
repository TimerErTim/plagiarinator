#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
// cleaned up version

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

namespace shapes {

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double radius) : radius_(radius) {}
    double area() const override {
        static constexpr double pi = 3.141592653589793;
        return pi * radius_ * radius_;
    }
    double perimeter() const override {
        static constexpr double pi = 3.141592653589793;
        return 2.0 * pi * radius_;
    }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2.0 * (w_ + h_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double side_;
public:
    explicit Triangle(double side) : side_(side) {}
    double area() const override {
        return std::sqrt(3.0) / 4.0 * side_ * side_;
    }
    double perimeter() const override { return 3.0 * side_; }
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
    std::vector<std::unique_ptr<Shape>> items;
    std::string label;
    while (std::cin >> label && label != "DONE") {
        if (label == "CIRCLE") {
            double r; std::cin >> r;
            items.emplace_back(std::make_unique<shapes::Circle>(r));
        } else if (label == "RECT") {
            double w, h; std::cin >> w >> h;
            items.emplace_back(std::make_unique<shapes::Rectangle>(w, h));
        } else if (label == "TRI") {
            double s; std::cin >> s;
            items.emplace_back(std::make_unique<shapes::Triangle>(s));
        }
    }

    double areaSum = 0.0, periSum = 0.0;
    for (const auto& ptr : items) {
        print_shape(*ptr);
        accumulate(*ptr, areaSum, periSum);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaSum << std::endl
              << "TOTAL perimeter=" << periSum << std::endl;
    return 0;
}
