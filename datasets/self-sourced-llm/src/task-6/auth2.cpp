#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
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
         << ", perimeter=" << s.perimeter() << endl;
}

void accumulate(const Shape& s, double& a, double& p)
{
    a += s.area();
    p += s.perimeter();
}

int main()
{
    vector<unique_ptr<Shape>> items;
    string tok;
    while (cin >> tok && tok != "DONE") {
        if (tok == "CIRCLE") {
            double r; cin >> r;
            items.push_back(make_unique<Circle>(r));
        } else if (tok == "RECT") {
            double w, h; cin >> w >> h;
            items.push_back(make_unique<Rectangle>(w, h));
        } else if (tok == "TRI") {
            double s; cin >> s;
            items.push_back(make_unique<Triangle>(s));
        }
    }
    double ta = 0, tp = 0;
    for (auto& sh : items) {
        print_shape(*sh);
        accumulate(*sh, ta, tp);
    }
    cout << fixed << setprecision(4)
         << "TOTAL area=" << ta << endl
         << "TOTAL perimeter=" << tp << endl;
    return 0;
}
