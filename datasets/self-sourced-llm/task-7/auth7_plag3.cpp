#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
// found this online lol

class Vehicle {
public:
    virtual ~Vehicle() {}
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Roadster : public Vehicle {
    double burn;
public:
    Roadster(double x): burn(x) {}
    double fuel_needed(double d) const override { return burn*d/100.0; }
    std::string type() const override { return "CAR"; }
};

class Lorry : public Vehicle {
    double burn;
public:
    Lorry(double x): burn(x) {}
    double fuel_needed(double d) const override { return burn*d; }
    std::string type() const override { return "TRUCK"; }
};

class Cycle : public Vehicle {
public:
    double fuel_needed(double) const override { return 0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> spawnRide(const std::string& tag, std::istream& src) {
    if (tag=="CAR"){double x;src>>x;return std::make_unique<Roadster>(x);}
    if (tag=="TRUCK"){double x;src>>x;return std::make_unique<Lorry>(x);}
    if (tag=="BIKE") return std::make_unique<Cycle>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> deck;
    std::vector<double> kmList;
    std::string tag;
    while(std::cin>>tag) {
        auto ride=spawnRide(tag,std::cin);
        if(!ride){std::string junk;std::getline(std::cin,junk);std::cout<<"ERROR\n";continue;}
        double km;std::cin>>km;
        deck.push_back(std::move(ride)); kmList.push_back(km);
    }
    double acc=0;
    for(size_t slot=0;slot<deck.size();++slot){
        double f=deck[slot]->fuel_needed(kmList[slot]); acc+=f;
        std::cout<<"Trip "<<slot+1<<": "<<deck[slot]->type()
                 <<" needs "<<std::fixed<<std::setprecision(2)<<f<<" liters\n";
    }
    std::cout<<"TOTAL "<<std::fixed<<std::setprecision(2)<<acc<<" liters\n";
    return 0;
}
