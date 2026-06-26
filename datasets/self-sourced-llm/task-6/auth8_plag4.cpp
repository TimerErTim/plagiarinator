#include <iomanip>
#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
// quick submit

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

namespace shapes {

class Circle : public Shape {
    double word_;
public:
    explicit Circle(double word) : word_(word) {}
    double area() const override {
        static constexpr double pi = 3.141592653589793;
        return pi * word_ * word_;
    }
    double perimeter() const override {
        static constexpr double pi = 3.141592653589793;
        return 2.0 * pi * word_;
    }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double elemW_, elemH_;
public:
    Rectangle(double w, double h) : elemW_(w), elemH_(h) {}
    double area() const override { return elemW_ * elemH_; }
    double perimeter() const override { return 2.0 * (elemW_ + elemH_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double tmp_;
public:
    explicit Triangle(double tmp) : tmp_(tmp) {}
    double area() const override {
        return std::sqrt(3.0) / 4.0 * tmp_ * tmp_;
    }
    double perimeter() const override { return 3.0 * tmp_; }
    std::string name() const override { return "Triangle"; }
};

} // namespace shapes

void print_shape(const Shape& s) {
    std::cout << std::fixed << std::setprecision(4)
              << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << '\n';
}

void accumulate(const Shape& s, double& area_sum, double& peri_sum) {
    area_sum += s.area();
    peri_sum += s.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> figures;
    std::string opt;
    while (std::cin >> opt && opt != "DONE") {
        if (opt == "CIRCLE") {
            double r; std::cin >> r;
            figures.push_back(std::make_unique<shapes::Circle>(r));
        } else if (opt == "RECT") {
            double w, h; std::cin >> w >> h;
            figures.push_back(std::make_unique<shapes::Rectangle>(w, h));
        } else if (opt == "TRI") {
            double s; std::cin >> s;
            figures.push_back(std::make_unique<shapes::Triangle>(s));
        }
    }

    double areaTotal = 0.0, periTotal = 0.0;
    for (const auto& fig : figures) {
        print_shape(*fig);
        accumulate(*fig, areaTotal, periTotal);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaTotal << '\n'
              << "TOTAL perimeter=" << periTotal << '\n';
    return 0;
}
