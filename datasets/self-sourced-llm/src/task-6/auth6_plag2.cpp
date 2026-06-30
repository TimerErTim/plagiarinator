#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

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
    double area() const override
    {
        return 3.141592653589793 * radius * radius;
    }
    double perimeter() const override
    {
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
    double area() const override
    {
        return std::sqrt(3.0) / 4.0 * sideLength * sideLength;
    }
    double perimeter() const override { return 3.0 * sideLength; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& shapeRef)
{
    std::cout << std::fixed << std::setprecision(4)
              << shapeRef.name() << ": area=" << shapeRef.area()
              << ", perimeter=" << shapeRef.perimeter() << std::endl;
}

void accumulate(const Shape& shapeRef, double& areaSum, double& perimeterSum)
{
    areaSum += shapeRef.area();
    perimeterSum += shapeRef.perimeter();
}

std::unique_ptr<Shape> extract(const std::string& command, std::istream& in)
{
    if (command == "CIRCLE")
    {
        double r; in >> r;
        return std::make_unique<Circle>(r);
    }
    if (command == "RECT")
    {
        double w, h; in >> w >> h;
        return std::make_unique<Rectangle>(w, h);
    }
    if (command == "TRI")
    {
        double s; in >> s;
        return std::make_unique<Triangle>(s);
    }
    return nullptr;
}

int main()
{
    std::stringstream input;
    input << std::cin.rdbuf();

    std::vector<std::unique_ptr<Shape>> shapes;
    std::string command;
    while (input >> command)
    {
        if (command == "DONE") break;
        auto fig = extract(command, input);
        if (fig) shapes.push_back(std::move(fig));
    }

    double totalArea = 0.0, totalPerimeter = 0.0;
    for (std::size_t idx = 0; idx < shapes.size(); ++idx)
    {
        print_shape(*shapes[idx]);
        accumulate(*shapes[idx], totalArea, totalPerimeter);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << totalArea << std::endl
              << "TOTAL perimeter=" << totalPerimeter << std::endl;
    return 0;
}
