#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
// copied from stackoverflow mostly

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double rVal;
public:
    explicit Circle(double r) : rVal(r) {}
    double area() const override { return 3.141592653589793 * rVal * rVal; }
    double perimeter() const override { return 2 * 3.141592653589793 * rVal; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double xLen, yLen;
public:
    Rectangle(double w, double h) : xLen(w), yLen(h) {}
    double area() const override { return xLen * yLen; }
    double perimeter() const override { return 2 * (xLen + yLen); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double sLen;
public:
    explicit Triangle(double s) : sLen(s) {}
    double area() const override { return std::sqrt(3.0) * sLen * sLen / 4.0; }
    double perimeter() const override { return 3 * sLen; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& fig) {
    std::cout << std::fixed << std::setprecision(4)
              << fig.name() << ": area=" << fig.area()
              << ", perimeter=" << fig.perimeter() << '\n';
}

void accumulate(const Shape& fig, double& aOut, double& pOut) {
    aOut += fig.area(); pOut += fig.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> store;
    std::string cmd;
    while (std::cin >> cmd && cmd != "DONE") {
        if (cmd == "CIRCLE") { double r; std::cin >> r; store.push_back(std::make_unique<Circle>(r)); }
        else if (cmd == "RECT") { double w,h; std::cin >> w >> h; store.push_back(std::make_unique<Rectangle>(w,h)); }
        else if (cmd == "TRI") { double s; std::cin >> s; store.push_back(std::make_unique<Triangle>(s)); }
    }
    double totA = 0, totP = 0;
    for (std::size_t n = 0; n < store.size(); ++n) {
        print_shape(*store[n]);
        accumulate(*store[n], totA, totP);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << totA << '\n' << "TOTAL perimeter=" << totP << '\n';
    return 0;
}
