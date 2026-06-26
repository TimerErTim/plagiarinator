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
    double perimeter() const override { return 2 * 3.141592653589793 * rad_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    double area() const override { return width_ * height_; }
    double perimeter() const override { return 2 * (width_ + height_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double side_;
public:
    explicit Triangle(double s) : side_(s) {}
    double area() const override { return std::sqrt(3.0) * side_ * side_ / 4.0; }
    double perimeter() const override { return 3 * side_; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& s) {
    std::cout << std::fixed << std::setprecision(4)
              << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << '\n';
}

void accumulate(const Shape& s, double& ar, double& pr) {
    ar += s.area(); pr += s.perimeter();
}

int main()
{
    std::vector<std::unique_ptr<Shape>> v;
    std::string t;
    while (std::cin >> t && t != "DONE")
{
        if (t == "CIRCLE")
{ double r; std::cin >> r; v.push_back(std::make_unique<Circle>(r)); }
        else if (t == "RECT")
{ double w,h; std::cin >> w >> h; v.push_back(std::make_unique<Rectangle>(w,h)); }
        else if (t == "TRI")
{ double s; std::cin >> s; v.push_back(std::make_unique<Triangle>(s)); }
    }
    double A=0,P=0;
    for (auto& p : v)
{ print_shape(*p); accumulate(*p,A,P); }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << A << '\n' << "TOTAL perimeter=" << P << '\n';
    return 0;
}
