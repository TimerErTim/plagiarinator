#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <vector>

namespace {
constexpr double kPi = 3.141592653589793;
}

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return kPi * radius_ * radius_; }
    double perimeter() const override { return 2.0 * kPi * radius_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double width_, height_;
public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    double area() const override { return width_ * height_; }
    double perimeter() const override { return 2.0 * (width_ + height_); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double edge_;
public:
    explicit Triangle(double e) : edge_(e) {}
    double area() const override
    {
        return std::sqrt(3.0) * edge_ * edge_ / 4.0;
    }
    double perimeter() const override { return 3.0 * edge_; }
    std::string name() const override { return "Triangle"; }
};

static void print_shape(const Shape& figure)
{
    std::cout << std::fixed << std::setprecision(4)
              << figure.name() << ": area=" << figure.area()
              << ", perimeter=" << figure.perimeter() << std::endl;
}

static void accumulate(const Shape& figure, double& area_sum, double& peri_sum)
{
    area_sum += figure.area();
    peri_sum += figure.perimeter();
}

std::unique_ptr<Shape> extract(const std::string& token, std::istream& in)
{
    if (token == "CIRCLE")
    {
        double r; in >> r;
        return std::make_unique<Circle>(r);
    }
    if (token == "RECT")
    {
        double w, h; in >> w >> h;
        return std::make_unique<Rectangle>(w, h);
    }
    if (token == "TRI")
    {
        double s; in >> s;
        return std::make_unique<Triangle>(s);
    }
    return nullptr;
}

int main()
{
    std::vector<std::unique_ptr<Shape>> gallery;
    std::string token;
    while (std::cin >> token)
    {
        if (token == "DONE") break;
        auto fig = extract(token, std::cin);
        if (fig) gallery.push_back(std::move(fig));
    }

    double areaTotal = 0.0, periTotal = 0.0;
    for (std::size_t idx = 0; idx < gallery.size(); ++idx)
    {
        print_shape(*gallery[idx]);
        accumulate(*gallery[idx], areaTotal, periTotal);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << areaTotal << std::endl
              << "TOTAL perimeter=" << periTotal << std::endl;
    return 0;
}
