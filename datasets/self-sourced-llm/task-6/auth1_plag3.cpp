#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
// borrowed from classmate solution

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double radiusVal;
public:
    explicit Circle(double r) : radiusVal(r) {}
    double area() const override { return 3.141592653589793 * radiusVal * radiusVal; }
    double perimeter() const override { return 2.0 * 3.141592653589793 * radiusVal; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double len, wid;
public:
    Rectangle(double w, double h) : len(w), wid(h) {}
    double area() const override { return len * wid; }
    double perimeter() const override { return 2.0 * (len + wid); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double sideLen;
public:
    explicit Triangle(double s) : sideLen(s) {}
    double area() const override { return std::sqrt(3.0) / 4.0 * sideLen * sideLen; }
    double perimeter() const override { return 3.0 * sideLen; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& fig) {
    std::cout << std::fixed << std::setprecision(4)
              << fig.name() << ": area=" << fig.area()
              << ", perimeter=" << fig.perimeter() << '\n';
}

void accumulate(const Shape& fig, double& aTot, double& pTot) {
    aTot += fig.area();
    pTot += fig.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> allShapes;
    std::string op;
    while (std::cin >> op && op != "DONE") {
        if (op == "CIRCLE") {
            double r; std::cin >> r;
            allShapes.push_back(std::make_unique<Circle>(r));
        } else if (op == "RECT") {
            double w, h; std::cin >> w >> h;
            allShapes.push_back(std::make_unique<Rectangle>(w, h));
        } else if (op == "TRI") {
            double s; std::cin >> s;
            allShapes.push_back(std::make_unique<Triangle>(s));
        }
    }

    double areaAcc = 0.0, periAcc = 0.0;
    for (std::size_t n = 0; n < allShapes.size(); ++n) {
        print_shape(*allShapes[n]);
        accumulate(*allShapes[n], areaAcc, periAcc);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaAcc << '\n'
              << "TOTAL perimeter=" << periAcc << '\n';
    return 0;
}
