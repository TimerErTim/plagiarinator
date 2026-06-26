#include <iomanip>
#include <cmath>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
// quick submit

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
    double word;
public:
    Circle(double r): word(r) {}
    double area() const override { return PI * word * word; }
    double perimeter() const override { return 2 * PI * word; }
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
    double tmp;
public:
    Triangle(double s): tmp(s) {}
    double area() const override { return sqrt(3.0) / 4.0 * tmp * tmp; }
    double perimeter() const override { return 3 * tmp; }
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
    vector<unique_ptr<Shape>> data;
    string tok;
    while (cin >> tok && tok != "DONE") {
        if (tok == "CIRCLE") {
            double r; cin >> r;
            data.push_back(make_unique<Circle>(r));
        } else if (tok == "RECT") {
            double w, h; cin >> w >> h;
            data.push_back(make_unique<Rectangle>(w, h));
        } else if (tok == "TRI") {
            double s; cin >> s;
            data.push_back(make_unique<Triangle>(s));
        }
    }
    double numA = 0, numP = 0;
    for (auto& sh : data) {
        print_shape(*sh);
        accumulate(*sh, numA, numP);
    }
    cout << fixed << setprecision(4)
         << "TOTAL area=" << numA << endl
         << "TOTAL perimeter=" << numP << endl;
    return 0;
}
