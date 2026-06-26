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

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.141592653589793 * r_ * r_; }
    double perimeter() const override { return 2.0 * 3.141592653589793 * r_; }
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
    double s_;
public:
    explicit Triangle(double s) : s_(s) {}
    double area() const override { return std::sqrt(3.0) / 4.0 * s_ * s_; }
    double perimeter() const override { return 3.0 * s_; }
    std::string name() const override { return "Triangle"; }
};

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
    std::vector<std::unique_ptr<Shape>> shapes;
    std::string token;
    while (std::cin >> token && token != "DONE") {
        if (token == "CIRCLE") {
            double r; std::cin >> r;
            shapes.emplace_back(std::make_unique<Circle>(r));
        } else if (token == "RECT") {
            double w, h; std::cin >> w >> h;
            shapes.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (token == "TRI") {
            double s; std::cin >> s;
            shapes.emplace_back(std::make_unique<Triangle>(s));
        }
    }

    double areaSum = 0.0, periSum = 0.0;
    for (const auto& ptr : shapes) {
        print_shape(*ptr);
        accumulate(*ptr, areaSum, periSum);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaSum << std::endl
              << "TOTAL perimeter=" << periSum << std::endl;
    return 0;
}
