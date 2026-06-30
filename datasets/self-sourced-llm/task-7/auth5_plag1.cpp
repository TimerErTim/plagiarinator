#include <vector>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Car : public Vehicle {
    double rate_;
public:
    explicit Car(double r) : rate_(r) {}
    double fuel_needed(double d) const override { return rate_ * d / 100.0; }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double rate_;
public:
    explicit Truck(double r) : rate_(r) {}
    double fuel_needed(double d) const override { return rate_ * d; }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& cfg) {
    if (kind == "CAR") { double x; cfg >> x; return std::make_unique<Car>(x); }
    if (kind == "TRUCK") { double x; cfg >> x; return std::make_unique<Truck>(x); }
    if (kind == "BIKE") return std::make_unique<Bike>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    std::vector<double> dists;
    for (std::string ln; std::getline(std::cin, ln);) {
        std::istringstream s(ln);
        std::string mode;
        s >> mode;
        auto v = create_vehicle(mode, s);
        if (!v) { std::cout << "ERROR" << std::endl; continue; }
        double d; s >> d;
        fleet.emplace_back(std::move(v));
        dists.push_back(d);
    }
    double total = 0;
    for (std::size_t i = 0; i < fleet.size(); i++) {
        double f = fleet[i]->fuel_needed(dists[i]);
        total += f;
        std::cout << "Trip " << i+1 << ": " << fleet[i]->type()
                  << " needs " << std::fixed << std::setprecision(2) << f << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2) << total << " liters" << std::endl;
    return 0;
}
