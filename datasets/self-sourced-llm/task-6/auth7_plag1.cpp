#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
// cleaned up version

const double PI = 3.141592653589793;

class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double r;
public:
    Circle(double x): r(x) {}
    double area() const override { return PI*r*r; }
    double perimeter() const override { return 2*PI*r; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double a,b;
public:
    Rectangle(double x,double y): a(x),b(y) {}
    double area() const override { return a*b; }
    double perimeter() const override { return 2*(a+b); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double s;
public:
    Triangle(double x): s(x) {}
    double area() const override { return sqrt(3.0)*s*s/4.0; }
    double perimeter() const override { return 3*s; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& x) {
    std::cout<<std::fixed<<std::setprecision(4)
             <<x.name()<<": area="<<x.area()
             <<", perimeter="<<x.perimeter()<<"\n";
}

void accumulate(const Shape& x,double& u,double& v) {
    u+=x.area(); v+=x.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> items;
    std::string k;
    while(std::cin>>k&&k!="DONE") {
        if(k=="CIRCLE"){double r;std::cin>>r;items.emplace_back(std::make_unique<Circle>(r));}
        else if(k=="RECT"){double w,h;std::cin>>w>>h;items.emplace_back(std::make_unique<Rectangle>(w,h));}
        else if(k=="TRI"){double s;std::cin>>s;items.emplace_back(std::make_unique<Triangle>(s));}
    }
    double areaSum=0,periSum=0;
    for(auto& e:items){print_shape(*e);accumulate(*e,areaSum,periSum);}
    std::cout<<std::fixed<<std::setprecision(4)
             <<"TOTAL area="<<areaSum<<"\n"<<"TOTAL perimeter="<<periSum<<"\n";
    return 0;
}
