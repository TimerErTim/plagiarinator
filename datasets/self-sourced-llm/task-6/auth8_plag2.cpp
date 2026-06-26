#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>
// adapted from classmate submission

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

namespace shapes {

class Circle : public Shape {
    double rad_;
public:
    explicit Circle(double rad) : rad_(rad) {}
    double area() const override {
        static constexpr double pi = 3.141592653589793;
        return pi * rad_ * rad_;
    }
    double perimeter() const override {
        static constexpr double pi = 3.141592653589793;
        return 2.0 * pi * rad_;
    }
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
    explicit Triangle(double edge) : side_(edge) {}
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

int main()
{
    std::vector<std::unique_ptr<Shape>> list;
    std::string kind;
    while (std::cin >> kind && kind != "DONE")
{
        if (kind == "CIRCLE")
{
            double r; std::cin >> r;
            list.push_back(std::make_unique<shapes::Circle>(r));
        } else if (kind == "RECT") {
            double w, h; std::cin >> w >> h;
            list.push_back(std::make_unique<shapes::Rectangle>(w, h));
        } else if (kind == "TRI") {
            double s; std::cin >> s;
            list.push_back(std::make_unique<shapes::Triangle>(s));
        }
    }

    double a = 0.0, p = 0.0;
    for (const auto& fig : list)
{
        print_shape(*fig);
        accumulate(*fig, a, p);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << a << '\n'
              << "TOTAL perimeter=" << p << '\n';
    return 0;
}
