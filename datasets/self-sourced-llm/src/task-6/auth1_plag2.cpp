#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
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
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.141592653589793 * r_ * r_; }
    double perimeter() const override { return 2.0 * 3.141592653589793 * r_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2.0 * (w_ + h_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double s_;
public:
    explicit Triangle(double s) : s_(s) {}
    double area() const override { return std::sqrt(3.0) / 4.0 * s_ * s_; }
    double perimeter() const override { return 3.0 * s_; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& s)
{
    std::cout << std::fixed << std::setprecision(4)
              << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << '\n';
}

void accumulate(const Shape& s, double& area_sum, double& peri_sum)
{
    area_sum += s.area();
    peri_sum += s.perimeter();
}

std::unique_ptr<Shape> extract(const std::string& kind, std::istream& in)
{
    if (kind == "CIRCLE")
    {
        double r; in >> r;
        return std::make_unique<Circle>(r);
    }
    if (kind == "RECT")
    {
        double w, h; in >> w >> h;
        return std::make_unique<Rectangle>(w, h);
    }
    if (kind == "TRI")
    {
        double s; in >> s;
        return std::make_unique<Triangle>(s);
    }
    return nullptr;
}

int main()
{
    std::vector<std::unique_ptr<Shape>> shapes;
    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "DONE") break;
        auto shape = extract(cmd, std::cin);
        if (shape) shapes.push_back(std::move(shape));
    }

    double totalA = 0.0, totalP = 0.0;
    for (std::size_t idx = 0; idx < shapes.size(); ++idx)
    {
        print_shape(*shapes[idx]);
        accumulate(*shapes[idx], totalA, totalP);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << totalA << '\n'
              << "TOTAL perimeter=" << totalP << '\n';
    return 0;
}
