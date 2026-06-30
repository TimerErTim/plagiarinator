#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>
#include <string>

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
    if (kind=="CAR"){double x;in>>x;return std::make_unique<Car>(x);}
    if (kind=="TRUCK"){double x;in>>x;return std::make_unique<Truck>(x);}
    if (kind=="BIKE") return std::make_unique<Bike>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    std::vector<double> dists;
    std::string tag;
    while(std::cin>>tag) {
        auto unit=create_vehicle(tag,std::cin);
        if(!unit){std::string rest;std::getline(std::cin,rest);std::cout<<"ERROR"<<std::endl;continue;}
        double d;std::cin>>d;
        fleet.emplace_back(std::move(unit)); dists.push_back(d);
    }
    double sum=0;
    for(size_t i=0;i<fleet.size();i++){
        double f=fleet[i]->fuel_needed(dists[i]); sum+=f;
        std::cout<<"Trip "<<i+1<<": "<<fleet[i]->type()
                 <<" needs "<<std::fixed<<std::setprecision(2)<<f<<" liters"<<std::endl;
    }
    std::cout<<"TOTAL "<<std::fixed<<std::setprecision(2)<<sum<<" liters"<<std::endl;
    return 0;
}
