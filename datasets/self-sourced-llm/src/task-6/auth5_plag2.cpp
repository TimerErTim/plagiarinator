#include <cmath>
#include <iostream>
#include <iomanip>
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
    double rad_;
public:
    explicit Circle(double r) : rad_(r) {}
    double area() const override { return 3.141592653589793 * rad_ * rad_; }
    double perimeter() const override { return 2 * 3.141592653589793 * rad_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2 * (w_ + h_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double side_;
public:
    explicit Triangle(double s) : side_(s) {}
    double area() const override { return std::sqrt(3.0) * side_ * side_ / 4.0; }
    double perimeter() const override { return 3 * side_; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& s)
{
    std::cout << std::fixed << std::setprecision(4)
              << s.name() << ": area=" << s.area()
              << ", perimeter=" << s.perimeter() << '\n';
}

void accumulate(const Shape& s, double& ar, double& pr)
{
    ar += s.area(); pr += s.perimeter();
}

std::unique_ptr<Shape> extract(const std::string& t, std::istream& in)
{
    if (t == "CIRCLE")
    {
        double r; in >> r;
        return std::make_unique<Circle>(r);
    }
    if (t == "RECT")
    {
        double w, h; in >> w >> h;
        return std::make_unique<Rectangle>(w, h);
    }
    if (t == "TRI")
    {
        double s; in >> s;
        return std::make_unique<Triangle>(s);
    }
    return nullptr;
}

int main()
{
    std::vector<std::unique_ptr<Shape>> v;
    std::string tok;
    while (std::cin >> tok)
    {
        if (tok == "DONE") break;
        auto p = extract(tok, std::cin);
        if (p) v.push_back(std::move(p));
    }
    double A = 0, P = 0;
    for (std::size_t idx = 0; idx < v.size(); ++idx)
    {
        print_shape(*v[idx]);
        accumulate(*v[idx], A, P);
    }
    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << A << '\n' << "TOTAL perimeter=" << P << '\n';
    return 0;
}
