#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>
// found this online and tweaked it

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
    double rad_;
public:
    explicit Circle(double r) : rad_(r) {}
    double area() const override { return kPi * rad_ * rad_; }
    double perimeter() const override { return 2.0 * kPi * rad_; }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double wLen, hLen;
public:
    Rectangle(double w, double h) : wLen(w), hLen(h) {}
    double area() const override { return wLen * hLen; }
    double perimeter() const override { return 2.0 * (wLen + hLen); }
    std::string name() const override { return "Rectangle"; }
};

class Triangle : public Shape {
    double sideLen;
public:
    explicit Triangle(double e) : sideLen(e) {}
    double area() const override {
        return std::sqrt(3.0) * sideLen * sideLen / 4.0;
    }
    double perimeter() const override { return 3.0 * sideLen; }
    std::string name() const override { return "Triangle"; }
};

static void print_shape(const Shape& fig) {
    std::cout << std::fixed << std::setprecision(4)
              << fig.name() << ": area=" << fig.area()
              << ", perimeter=" << fig.perimeter() << std::endl;
}

static void accumulate(const Shape& fig, double& aSum, double& pSum) {
    aSum += fig.area();
    pSum += fig.perimeter();
}

int main() {
    std::vector<std::unique_ptr<Shape>> coll;
    std::string tok;
    while (std::cin >> tok) {
        if (tok == "DONE") break;
        if (tok == "CIRCLE") {
            double r; std::cin >> r;
            coll.emplace_back(std::make_unique<Circle>(r));
        } else if (tok == "RECT") {
            double w, h; std::cin >> w >> h;
            coll.emplace_back(std::make_unique<Rectangle>(w, h));
        } else if (tok == "TRI") {
            double s; std::cin >> s;
            coll.emplace_back(std::make_unique<Triangle>(s));
        }
    }

    double totA = 0.0, totP = 0.0;
    for (std::size_t j = 0; j < coll.size(); ++j) {
        print_shape(*coll[j]);
        accumulate(*coll[j], totA, totP);
    }

    std::cout << std::fixed << std::setprecision(4)
              << "TOTAL area=" << totA << std::endl
              << "TOTAL perimeter=" << totP << std::endl;
    return 0;
}
