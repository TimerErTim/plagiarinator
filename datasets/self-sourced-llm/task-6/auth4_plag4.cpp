#include <iomanip>
#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
// quick submit

#define PI_VAL 3.141592653589793

class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double r;
public:
    Circle(double word) : r(word) {}
    double area() const override { return PI_VAL * r * r; }
    double perimeter() const override { return 2 * PI_VAL * r; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double w, h;
public:
    Rectangle(double width, double height) : w(width), h(height) {}
    double area() const override { return w * h; }
    double perimeter() const override { return 2 * (w + h); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double tmp;
public:
    Triangle(double s) : tmp(s) {}
    double area() const override { return (std::sqrt(3.0) / 4.0) * tmp * tmp; }
    double perimeter() const override { return 3.0 * tmp; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& s) {
    std::cout << std::fixed << std::setprecision(4)
              << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << '\n';
}

void accumulate(const Shape& s, double& a_sum, double& p_sum) {
    a_sum += s.area();
    p_sum += s.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> collection;
    std::string opt;
    while (std::cin >> opt && opt != "DONE") {
        if (opt == "CIRCLE") {
            double rad; std::cin >> rad;
            collection.push_back(std::unique_ptr<Shape>(new Circle(rad)));
        } else if (opt == "RECT") {
            double w, h; std::cin >> w >> h;
            collection.push_back(std::unique_ptr<Shape>(new Rectangle(w, h)));
        } else if (opt == "TRI") {
            double s; std::cin >> s;
            collection.push_back(std::unique_ptr<Shape>(new Triangle(s)));
        }
    }

    double numA = 0, numP = 0;
    for (size_t i = 0; i < collection.size(); ++i) {
        print_shape(*collection[i]);
        accumulate(*collection[i], numA, numP);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << numA << '\n'
              << "TOTAL perimeter=" << numP << '\n';
    return 0;
}
