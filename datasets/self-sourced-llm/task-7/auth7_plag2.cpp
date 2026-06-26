#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
// adapted from classmate submission

class Vehicle {
public:
    virtual ~Vehicle() {}
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Car : public Vehicle {
    double r;
public:
    Car(double x): r(x) {}
    double fuel_needed(double d) const override { return r*d/100.0; }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double r;
public:
    Truck(double x): r(x) {}
    double fuel_needed(double d) const override { return r*d; }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& in) {
    if (kind=="CAR")
{double x;in>>x;return std::make_unique<Car>(x);}
    if (kind=="TRUCK")
{double x;in>>x;return std::make_unique<Truck>(x);}
    if (kind=="BIKE") return std::make_unique<Bike>();
    return nullptr;
}

int main()
{
    std::vector<std::unique_ptr<Vehicle>> V;
    std::vector<double> D;
    std::string k;
    while(std::cin>>k)
{
        auto p=create_vehicle(k,std::cin);
        if(!p)
{std::string rest;std::getline(std::cin,rest);std::cout<<"ERROR\n";continue;}
        double d;std::cin>>d;
        V.push_back(std::move(p)); D.push_back(d);
    }
    double s=0;
    for(size_t i=0;i<V.size();++i){
        double f=V[i]->fuel_needed(D[i]); s+=f;
        std::cout<<"Trip "<<i+1<<": "<<V[i]->type()
                 <<" needs "<<std::fixed<<std::setprecision(2)<<f<<" liters\n";
    }
    std::cout<<"TOTAL "<<std::fixed<<std::setprecision(2)<<s<<" liters\n";
    return 0;
}
