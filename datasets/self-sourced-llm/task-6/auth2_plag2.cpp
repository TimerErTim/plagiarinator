#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>
// adapted from classmate submission

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
    double rad;
public:
    Circle(double r): rad(r) {}
    double area() const override { return PI * rad * rad; }
    double perimeter() const override { return 2 * PI * rad; }
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
    double edge;
public:
    Triangle(double s): edge(s) {}
    double area() const override { return sqrt(3.0) / 4.0 * edge * edge; }
    double perimeter() const override { return 3 * edge; }
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
    vector<unique_ptr<Shape>> list;
    string tok;
    while (cin >> tok && tok != "DONE")
{
        if (tok == "CIRCLE")
{
            double r; cin >> r;
            list.push_back(make_unique<Circle>(r));
        } else if (tok == "RECT") {
            double w, h; cin >> w >> h;
            list.push_back(make_unique<Rectangle>(w, h));
        } else if (tok == "TRI") {
            double s; cin >> s;
            list.push_back(make_unique<Triangle>(s));
        }
    }
    double a = 0, p = 0;
    for (auto& sh : list)
{
        print_shape(*sh);
        accumulate(*sh, a, p);
    }
    cout << fixed << setprecision(4)
         << "TOTAL area=" << a << endl
         << "TOTAL perimeter=" << p << endl;
    return 0;
}
