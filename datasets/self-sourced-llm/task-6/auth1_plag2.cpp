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

class Circle : public Shape {
    double rad_;
public:
    explicit Circle(double r) : rad_(r) {}
    double area() const override { return 3.141592653589793 * rad_ * rad_; }
    double perimeter() const override { return 2.0 * 3.141592653589793 * rad_; }
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
    explicit Triangle(double s) : side_(s) {}
    double area() const override { return std::sqrt(3.0) / 4.0 * side_ * side_; }
    double perimeter() const override { return 3.0 * side_; }
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

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    std::string op;
    while (std::cin >> op && op != "DONE")
{
        if (op == "CIRCLE")
{
            double r; std::cin >> r;
            shapes.push_back(std::make_unique<Circle>(r));
        } else if (op == "RECT") {
            double w, h; std::cin >> w >> h;
            shapes.push_back(std::make_unique<Rectangle>(w, h));
        } else if (op == "TRI") {
            double s; std::cin >> s;
            shapes.push_back(std::make_unique<Triangle>(s));
        }
    }

    double a = 0.0, p = 0.0;
    for (const auto& sp : shapes)
{
        print_shape(*sp);
        accumulate(*sp, a, p);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << a << '\n'
              << "TOTAL perimeter=" << p << '\n';
    return 0;
}
