#include <iostream>
#include <iomanip>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::fixed;
using std::make_unique;
using std::setprecision;
using std::sqrt;
using std::string;
using std::unique_ptr;
using std::vector;

static const double PI = 3.141592653589793;

class Shape
{
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual string name() const = 0;
};

class Circle : public Shape
{
    double radius;
public:
    Circle(double r): radius(r) {}
    double area() const override { return PI * radius * radius; }
    double perimeter() const override { return 2 * PI * radius; }
    string name() const override { return "Circle"; }
};

class Rectangle : public Shape
{
    double w, h;
public:
    Rectangle(double width, double height): w(width), h(height) {}
    double area() const override { return w * h; }
    double perimeter() const override { return 2 * (w + h); }
    string name() const override { return "Rectangle"; }
};

class Triangle : public Shape
{
    double side;
public:
    Triangle(double s): side(s) {}
    double area() const override { return sqrt(3.0) / 4.0 * side * side; }
    double perimeter() const override { return 3 * side; }
    string name() const override { return "Triangle"; }
};

void print_shape(const Shape& s)
{
    cout << fixed << setprecision(4)
         << s.name() << ": area=" << s.area()
         << ", perimeter=" << s.perimeter() << '\n';
}

void accumulate(const Shape& s, double& a, double& p)
{
    a += s.area();
    p += s.perimeter();
}

unique_ptr<Shape> extract(const string& tok, std::istream& in)
{
    if (tok == "CIRCLE")
    {
        double r; in >> r;
        return make_unique<Circle>(r);
    }
    if (tok == "RECT")
    {
        double w, h; in >> w >> h;
        return make_unique<Rectangle>(w, h);
    }
    if (tok == "TRI")
    {
        double s; in >> s;
        return make_unique<Triangle>(s);
    }
    return nullptr;
}

int main()
{
    vector<unique_ptr<Shape>> items;
    string tok;
    while (cin >> tok)
    {
        if (tok == "DONE") break;
        auto sh = extract(tok, cin);
        if (sh) items.push_back(std::move(sh));
    }
    double ta = 0, tp = 0;
    for (size_t idx = 0; idx < items.size(); ++idx)
    {
        print_shape(*items[idx]);
        accumulate(*items[idx], ta, tp);
    }
    cout << fixed << setprecision(4)
         << "TOTAL area=" << ta << '\n'
         << "TOTAL perimeter=" << tp << '\n';
    return 0;
}
