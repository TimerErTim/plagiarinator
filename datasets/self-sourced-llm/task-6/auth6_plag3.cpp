#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
// lab partner helped with this one

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double rad;
public:
    explicit Circle(double r) : rad(r) {}
    double area() const override {
        return 3.141592653589793 * rad * rad;
    }
    double perimeter() const override {
        return 2.0 * 3.141592653589793 * rad;
    }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double wDim, hDim;
public:
    Rectangle(double w, double h) : wDim(w), hDim(h) {}
    double area() const override { return wDim * hDim; }
    double perimeter() const override { return 2.0 * (wDim + hDim); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double side;
public:
    explicit Triangle(double s) : side(s) {}
    double area() const override {
        return std::sqrt(3.0) / 4.0 * side * side;
    }
    double perimeter() const override { return 3.0 * side; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& ref) {
    std::cout << std::fixed << std::setprecision(4)
              << ref.name() << ": area=" << ref.area()
              << ", perimeter=" << ref.perimeter() << std::endl;
}

void accumulate(const Shape& ref, double& aAcc, double& pAcc) {
    aAcc += ref.area();
    pAcc += ref.perimeter();
}

int main() {
    std::stringstream buf;
    buf << std::cin.rdbuf();

    std::vector<std::unique_ptr<Shape>> list;
    std::string cmd;
    while (buf >> cmd && cmd != "DONE") {
        if (cmd == "CIRCLE") {
            double r; buf >> r;
            list.push_back(std::make_unique<Circle>(r));
        } else if (cmd == "RECT") {
            double w, h; buf >> w >> h;
            list.push_back(std::make_unique<Rectangle>(w, h));
        } else if (cmd == "TRI") {
            double s; buf >> s;
            list.push_back(std::make_unique<Triangle>(s));
        }
    }

    double aTot = 0.0, pTot = 0.0;
    for (std::size_t k = 0; k < list.size(); ++k) {
        print_shape(*list[k]);
        accumulate(*list[k], aTot, pTot);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << aTot << std::endl
              << "TOTAL perimeter=" << pTot << std::endl;
    return 0;
}
