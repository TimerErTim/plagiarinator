#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
// should work fine lol

const double kPiConst = 3.141592653589793;

class Shape {
public:
    virtual ~Shape() {}
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double rad;
public:
    Circle(double x): rad(x) {}
    double area() const override { return kPiConst*rad*rad; }
    double perimeter() const override { return 2*kPiConst*rad; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double len, wid;
public:
    Rectangle(double x,double y): len(x),wid(y) {}
    double area() const override { return len*wid; }
    double perimeter() const override { return 2*(len+wid); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double edge;
public:
    Triangle(double x): edge(x) {}
    double area() const override { return sqrt(3.0)*edge*edge/4.0; }
    double perimeter() const override { return 3*edge; }
    std::string name() const override { return "Triangle"; }
};

void print_shape(const Shape& fig) {
    std::cout<<std::fixed<<std::setprecision(4)
             <<fig.name()<<": area="<<fig.area()
             <<", perimeter="<<fig.perimeter()<<"\n";
}

void accumulate(const Shape& fig,double& aTot,double& pTot) {
    aTot+=fig.area(); pTot+=fig.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> figs;
    std::string word;
    while(std::cin>>word&&word!="DONE") {
        if(word=="CIRCLE"){double r;std::cin>>r;figs.push_back(std::make_unique<Circle>(r));}
        else if(word=="RECT"){double w,h;std::cin>>w>>h;figs.push_back(std::make_unique<Rectangle>(w,h));}
        else if(word=="TRI"){double s;std::cin>>s;figs.push_back(std::make_unique<Triangle>(s));}
    }
    double sumA=0,sumP=0;
    for(std::size_t n=0;n<figs.size();++n){
        print_shape(*figs[n]);
        accumulate(*figs[n],sumA,sumP);
    }
    std::cout<<std::fixed<<std::setprecision(4)
             <<"TOTAL area="<<sumA<<"\n"<<"TOTAL perimeter="<<sumP<<"\n";
    return 0;
}
