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
    double perimeter() const override { return 2.0 * 3.141592653589793 * ref; }
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
    explicit Triangle(double s) : val(s) {}
    double area() const override { return std::sqrt(3.0) / 4.0 * val * val; }
    double perimeter() const override { return 3.0 * val; }
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
    std::string line;
    while (std::cin >> line && line != "DONE") {
        if (line == "CIRCLE") {
            double r; std::cin >> r;
            shapes.push_back(std::make_unique<Circle>(r));
        } else if (line == "RECT") {
            double w, h; std::cin >> w >> h;
            shapes.push_back(std::make_unique<Rectangle>(w, h));
        } else if (line == "TRI") {
            double s; std::cin >> s;
            shapes.push_back(std::make_unique<Triangle>(s));
        }
    }

    double totalA = 0.0, totalP = 0.0;
    for (const auto& sp : shapes) {
        print_shape(*sp);
        accumulate(*sp, totalA, totalP);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << totalA << std::endl
              << "TOTAL perimeter=" << totalP << std::endl;
    return 0;
}
