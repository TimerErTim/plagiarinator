#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include <iomanip>
#include <iostream>

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
             <<", perimeter="<<x.perimeter()<<std::endl;
}

void accumulate(const Shape& x,double& u,double& v) {
    u+=x.area(); v+=x.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> data;
    std::string k;
    while(std::cin>>k&&k!="DONE") {
        if(k=="CIRCLE"){double r;std::cin>>r;data.emplace_back(std::make_unique<Circle>(r));}
        else if(k=="RECT"){double w,h;std::cin>>w>>h;data.emplace_back(std::make_unique<Rectangle>(w,h));}
        else if(k=="TRI"){double s;std::cin>>s;data.emplace_back(std::make_unique<Triangle>(s));}
    }
    double ta=0,tp=0;
    for(size_t i=0;i<data.size();i++){print_shape(*data[i]);accumulate(*data[i],ta,tp);}
    std::cout<<std::fixed<<std::setprecision(4)
             <<"TOTAL area="<<ta<<std::endl<<"TOTAL perimeter="<<tp<<std::endl;
    return 0;
}
