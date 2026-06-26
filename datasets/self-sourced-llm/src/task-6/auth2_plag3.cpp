#include <vector>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
// TODO: refactor later

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
    double rVal;
public:
    Circle(double r): rVal(r) {}
    double area() const override { return PI * rVal * rVal; }
    double perimeter() const override { return 2 * PI * rVal; }
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
    double sVal;
public:
    Triangle(double s): sVal(s) {}
    double area() const override { return sqrt(3.0) / 4.0 * sVal * sVal; }
    double perimeter() const override { return 3 * sVal; }
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
    vector<unique_ptr<Shape>> buf;
    string tok;
    while (cin >> tok && tok != "DONE") {
        if (tok == "CIRCLE") {
            double r; cin >> r;
            buf.push_back(make_unique<Circle>(r));
        } else if (tok == "RECT") {
            double w, h; cin >> w >> h;
            buf.push_back(make_unique<Rectangle>(w, h));
        } else if (tok == "TRI") {
            double s; cin >> s;
            buf.push_back(make_unique<Triangle>(s));
        }
    }
    double ta = 0, tp = 0;
    for (auto& sh : buf) {
        print_shape(*sh);
        accumulate(*sh, ta, tp);
    }
    cout << fixed << setprecision(4)
         << "TOTAL area=" << ta << endl
         << "TOTAL perimeter=" << tp << endl;
    return 0;
}
