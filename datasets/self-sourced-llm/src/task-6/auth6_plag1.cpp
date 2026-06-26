#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
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
    double radius;
public:
    explicit Circle(double r) : radius(r) {}
    double area() const override {
        return 3.141592653589793 * radius * radius;
    }
    double perimeter() const override {
        return 2.0 * 3.141592653589793 * radius;
    }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override { return width * height; }
    double perimeter() const override { return 2.0 * (width + height); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double sideLength;
public:
    explicit Triangle(double s) : sideLength(s) {}
    double area() const override {
        return std::sqrt(3.0) / 4.0 * sideLength * sideLength;
    }
    double perimeter() const override { return 3.0 * sideLength; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& shapeRef) {
    std::cout << std::fixed << std::setprecision(4)
              << shapeRef.name() << ": area=" << shapeRef.area()
              << ", perimeter=" << shapeRef.perimeter() << std::endl;
}

void accumulate(const Shape& shapeRef, double& areaSum, double& perimeterSum) {
    areaSum += shapeRef.area();
    perimeterSum += shapeRef.perimeter();
}

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    std::vector<std::unique_ptr<Shape>> shapes;
    std::string token;
    while (input >> token && token != "DONE") {
        if (token == "CIRCLE") {
            double r; input >> r;
            shapes.emplace_back(std::make_unique<Circle>(r));
        } else if (token == "RECT") {
            double w, h; input >> w >> h;
            shapes.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (token == "TRI") {
            double s; input >> s;
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
