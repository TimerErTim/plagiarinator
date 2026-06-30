#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double rad_;
public:
    explicit Circle(double r) : rad_(r) {}
    double area() const override { return 3.141592653589793 * rad_ * rad_; }
    double perimeter() const override { return 2.0 * 3.141592653589793 * rad_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double wid_, ht_;
public:
    Rectangle(double w, double h) : wid_(w), ht_(h) {}
    double area() const override { return wid_ * ht_; }
    double perimeter() const override { return 2.0 * (wid_ + ht_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double edge_;
public:
    explicit Triangle(double s) : edge_(s) {}
    double area() const override { return std::sqrt(3.0) / 4.0 * edge_ * edge_; }
    double perimeter() const override { return 3.0 * edge_; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& s) {
    std::cout << std::fixed << std::setprecision(4)
              << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << std::endl;
}

void accumulate(const Shape& s, double& area_sum, double& peri_sum) {
    area_sum += s.area();
    peri_sum += s.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> figList;
    std::string token;
    while (std::cin >> token && token != "DONE") {
        if (token == "CIRCLE") {
            double r; std::cin >> r;
            figList.emplace_back(std::make_unique<Circle>(r));
        } else if (token == "RECT") {
            double w, h; std::cin >> w >> h;
            figList.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (token == "TRI") {
            double s; std::cin >> s;
            figList.emplace_back(std::make_unique<Triangle>(s));
        }
    }

    double sumArea = 0.0, sumPeri = 0.0;
    for (std::size_t i = 0; i < figList.size(); i++) {
        print_shape(*figList[i]);
        accumulate(*figList[i], sumArea, sumPeri);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << sumArea << std::endl
              << "TOTAL perimeter=" << sumPeri << std::endl;
    return 0;
}
