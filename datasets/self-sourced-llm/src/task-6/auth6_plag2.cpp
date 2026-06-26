#include <vector>
#include <string>
#include <sstream>
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
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    double area() const override { return width * height; }
    double perimeter() const override { return 2.0 * (width + height); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double edge;
public:
    explicit Triangle(double s) : edge(s) {}
    double area() const override {
        return std::sqrt(3.0) / 4.0 * edge * edge;
    }
    double perimeter() const override { return 3.0 * edge; }
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

int main()
{
    std::stringstream input;
    input << std::cin.rdbuf();

    std::vector<std::unique_ptr<Shape>> shapes;
    std::string command;
    while (input >> command && command != "DONE")
{
        if (command == "CIRCLE")
{
            double r; input >> r;
            shapes.emplace_back(std::make_unique<Circle>(r));
        } else if (command == "RECT") {
            double w, h; input >> w >> h;
            shapes.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (command == "TRI") {
            double s; input >> s;
            shapes.emplace_back(std::make_unique<Triangle>(s));
        }
    }

    double a = 0.0, p = 0.0;
    for (const auto& shapePtr : shapes)
{
        print_shape(*shapePtr);
        accumulate(*shapePtr, a, p);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << a << std::endl
              << "TOTAL perimeter=" << p << std::endl;
    return 0;
}
