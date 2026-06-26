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

class Circle : public Shape {
    double ref;
public:
    explicit Circle(double r) : ref(r) {}
    double area() const override { return 3.141592653589793 * ref * ref; }
    double perimeter() const override { return 2 * 3.141592653589793 * ref; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double argW, argH;
public:
    Rectangle(double w, double h) : argW(w), argH(h) {}
    double area() const override { return argW * argH; }
    double perimeter() const override { return 2 * (argW + argH); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double val;
public:
    explicit Triangle(double s) : val(s) {}
    double area() const override { return std::sqrt(3.0) * val * val / 4.0; }
    double perimeter() const override { return 3 * val; }
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

int main() {
    std::vector<std::unique_ptr<Shape>> v;
    std::string hold;
    while (std::cin >> hold && hold != "DONE") {
        if (hold == "CIRCLE") { double r; std::cin >> r; v.push_back(std::make_unique<Circle>(r)); }
        else if (hold == "RECT") { double w,h; std::cin >> w >> h; v.push_back(std::make_unique<Rectangle>(w,h)); }
        else if (hold == "TRI") { double s; std::cin >> s; v.push_back(std::make_unique<Triangle>(s)); }
    }
    double A=0,P=0;
    for (auto& p : v) { print_shape(*p); accumulate(*p,A,P); }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << A << std::endl << "TOTAL perimeter=" << P << std::endl;
    return 0;
}
