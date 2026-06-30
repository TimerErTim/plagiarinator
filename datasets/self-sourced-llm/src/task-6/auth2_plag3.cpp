#include <memory>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <string>
#include <vector>
// quick fix before deadline

using std::cin;
using std::cout;
using std::fixed;
using std::make_unique;
using std::setprecision;
using std::sqrt;
using std::string;
using std::unique_ptr;
using std::vector;

static const double kPiVal = 3.141592653589793;

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
    double radLen;
public:
    Circle(double r): radLen(r) {}
    double area() const override { return kPiVal * radLen * radLen; }
    double perimeter() const override { return 2 * kPiVal * radLen; }
    string name() const override { return "Circle"; }
};

class Rectangle : public Shape
{
    double xDim, yDim;
public:
    Rectangle(double width, double height): xDim(width), yDim(height) {}
    double area() const override { return xDim * yDim; }
    double perimeter() const override { return 2 * (xDim + yDim); }
    string name() const override { return "Rectangle"; }
};

class Triangle : public Shape
{
    double edgeLen;
public:
    Triangle(double s): edgeLen(s) {}
    double area() const override { return sqrt(3.0) / 4.0 * edgeLen * edgeLen; }
    double perimeter() const override { return 3 * edgeLen; }
    string name() const override { return "Triangle"; }
};

void print_shape(const Shape& fig)
{
    cout << fixed << setprecision(4)
         << fig.name() << ": area=" << fig.area()
         << ", perimeter=" << fig.perimeter() << '\n';
}

void accumulate(const Shape& fig, double& areaTot, double& periTot)
{
    areaTot += fig.area();
    periTot += fig.perimeter();
}

int main()
{
    vector<unique_ptr<Shape>> shapeBag;
    string word;
    while (cin >> word && word != "DONE") {
        if (word == "CIRCLE") {
            double r; cin >> r;
            shapeBag.push_back(make_unique<Circle>(r));
        } else if (word == "RECT") {
            double w, h; cin >> w >> h;
            shapeBag.push_back(make_unique<Rectangle>(w, h));
        } else if (word == "TRI") {
            double s; cin >> s;
            shapeBag.push_back(make_unique<Triangle>(s));
        }
    }
    double sumA = 0, sumP = 0;
    for (size_t k = 0; k < shapeBag.size(); ++k) {
        print_shape(*shapeBag[k]);
        accumulate(*shapeBag[k], sumA, sumP);
    }
    cout << fixed << setprecision(4)
         << "TOTAL area=" << sumA << '\n'
         << "TOTAL perimeter=" << sumP << '\n';
    return 0;
}
