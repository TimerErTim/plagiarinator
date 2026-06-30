#include <vector>
#include <memory>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
// my version of the shapes hw

#define PI_VAL 3.141592653589793

class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double radVal;
public:
    Circle(double rad) : radVal(rad) {}
    double area() const override { return PI_VAL * radVal * radVal; }
    double perimeter() const override { return 2 * PI_VAL * radVal; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double wid, ht;
public:
    Rectangle(double width, double height) : wid(width), ht(height) {}
    double area() const override { return wid * ht; }
    double perimeter() const override { return 2 * (wid + ht); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double edge;
public:
    Triangle(double s) : edge(s) {}
    double area() const override { return (std::sqrt(3.0) / 4.0) * edge * edge; }
    double perimeter() const override { return 3.0 * edge; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& fig) {
    std::cout << std::fixed << std::setprecision(4)
              << fig.name() << ": area=" << fig.area()
              << ", perimeter=" << fig.perimeter() << '\n';
}

void accumulate(const Shape& fig, double& arSum, double& prSum) {
    arSum += fig.area();
    prSum += fig.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> bag;
    std::string tag;
    while (std::cin >> tag && tag != "DONE") {
        if (tag == "CIRCLE") {
            double rad; std::cin >> rad;
            bag.push_back(std::unique_ptr<Shape>(new Circle(rad)));
        } else if (tag == "RECT") {
            double w, h; std::cin >> w >> h;
            bag.push_back(std::unique_ptr<Shape>(new Rectangle(w, h)));
        } else if (tag == "TRI") {
            double s; std::cin >> s;
            bag.push_back(std::unique_ptr<Shape>(new Triangle(s)));
        }
    }

    double areaTot = 0, periTot = 0;
    for (std::size_t n = 0; n < bag.size(); ++n) {
        print_shape(*bag[n]);
        accumulate(*bag[n], areaTot, periTot);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaTot << '\n'
              << "TOTAL perimeter=" << periTot << '\n';
    return 0;
}
