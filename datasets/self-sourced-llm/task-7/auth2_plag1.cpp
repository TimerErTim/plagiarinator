#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
// cleaned up version

using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::getline;
using std::istringstream;
using std::istream;
using std::make_unique;
using std::move;
using std::setprecision;
using std::string;
using std::unique_ptr;
using std::vector;
using size_t = std::size_t;

class Vehicle
{
public:
    virtual ~Vehicle() {}
    virtual double fuel_needed(double km) const = 0;
    virtual string type() const = 0;
};

class Car : public Vehicle
{
    double rate;
public:
    Car(double r): rate(r) {}
    double fuel_needed(double km) const override { return rate * km / 100.0; }
    string type() const override { return "CAR"; }
};

class Truck : public Vehicle
{
    double rate;
public:
    Truck(double r): rate(r) {}
    double fuel_needed(double km) const override { return rate * km; }
    string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle
{
public:
    double fuel_needed(double) const override { return 0.0; }
    string type() const override { return "BIKE"; }
};

unique_ptr<Vehicle> create_vehicle(const string& kind, istream& cfg)
{
    if (kind == "CAR") {
        double x; cfg >> x;
        return make_unique<Car>(x);
    }
    if (kind == "TRUCK") {
        double x; cfg >> x;
        return make_unique<Truck>(x);
    }
    if (kind == "BIKE")
        return make_unique<Bike>();
    return nullptr;
}

int main()
{
    vector<unique_ptr<Vehicle>> tripList;
    vector<double> kms;
    string s;
    while (getline(cin, s)) {
        istringstream in(s);
        string mode; in >> mode;
        auto v = create_vehicle(mode, in);
        if (!v) { cout << "ERROR" << endl; continue; }
        double d; in >> d;
        tripList.push_back(move(v));
        kms.push_back(d);
    }
    double sum = 0;
    for (size_t i = 0; i < tripList.size(); ++i) {
        double f = tripList[i]->fuel_needed(kms[i]);
        sum += f;
        cout << "Trip " << i + 1 << ": " << tripList[i]->type()
             << " needs " << fixed << setprecision(2) << f << " liters" << endl;
    }
    cout << "TOTAL " << fixed << setprecision(2) << sum << " liters" << endl;
    return 0;
}
