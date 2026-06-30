#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
// got this from a friend

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

namespace geo {

class Circle : public Shape {
    double rad_;
public:
    explicit Circle(double radius) : rad_(radius) {}
    double area() const override {
        static constexpr double pi = 3.141592653589793;
        return pi * rad_ * rad_;
    }
    double perimeter() const override {
        static constexpr double pi = 3.141592653589793;
        return 2.0 * pi * rad_;
    }
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
    explicit Triangle(double side) : edge_(side) {}
    double area() const override {
        return std::sqrt(3.0) / 4.0 * edge_ * edge_;
    }
    double perimeter() const override { return 3.0 * edge_; }
    std::string name() const override { return "Triangle"; }
};

} // namespace geo

void print_shape(const Shape& fig) {
    std::cout << std::fixed << std::setprecision(4)
              << fig.name() << ": area=" << fig.area()
              << ", perimeter=" << fig.perimeter() << '\n';
}

void accumulate(const Shape& fig, double& aSum, double& pSum) {
    aSum += fig.area();
    pSum += fig.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> items;
    std::string tok;
    while (std::cin >> tok && tok != "DONE") {
        if (tok == "CIRCLE") {
            double r; std::cin >> r;
            items.push_back(std::make_unique<geo::Circle>(r));
        } else if (tok == "RECT") {
            double w, h; std::cin >> w >> h;
            items.push_back(std::make_unique<geo::Rectangle>(w, h));
        } else if (tok == "TRI") {
            double s; std::cin >> s;
            items.push_back(std::make_unique<geo::Triangle>(s));
        }
    }

    double totA = 0.0, totP = 0.0;
    for (std::size_t j = 0; j < items.size(); ++j) {
        print_shape(*items[j]);
        accumulate(*items[j], totA, totP);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << totA << '\n'
              << "TOTAL perimeter=" << totP << '\n';
    return 0;
}
