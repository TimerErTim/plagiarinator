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

class Circle : public Shape {
    double word_;
public:
    explicit Circle(double r) : word_(r) {}
    double area() const override { return 3.141592653589793 * word_ * word_; }
    double perimeter() const override { return 2.0 * 3.141592653589793 * word_; }
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
    explicit Triangle(double s) : tmp_(s) {}
    double area() const override { return std::sqrt(3.0) / 4.0 * tmp_ * tmp_; }
    double perimeter() const override { return 3.0 * tmp_; }
    std::string name() const override { return "Triangle"; }
};

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
    std::vector<std::unique_ptr<Shape>> shapes;
    std::string word;
    while (std::cin >> word && word != "DONE") {
        if (word == "CIRCLE") {
            double r; std::cin >> r;
            shapes.push_back(std::make_unique<Circle>(r));
        } else if (word == "RECT") {
            double w, h; std::cin >> w >> h;
            shapes.push_back(std::make_unique<Rectangle>(w, h));
        } else if (word == "TRI") {
            double s; std::cin >> s;
            shapes.push_back(std::make_unique<Triangle>(s));
        }
    }

    double totalA = 0.0, totalP = 0.0;
    for (const auto& sp : shapes) {
        print_shape(*sp);
        accumulate(*sp, totalA, totalP);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << totalA << '\n'
              << "TOTAL perimeter=" << totalP << '\n';
    return 0;
}
