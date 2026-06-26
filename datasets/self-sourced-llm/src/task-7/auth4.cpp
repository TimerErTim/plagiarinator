#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Vehicle {
public:
    virtual ~Vehicle() {}
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Car : public Vehicle {
    double l100;
public:
    Car(double x) : l100(x) {}
    double fuel_needed(double d) const override { return l100 * d / 100.0; }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double lkm;
public:
    Truck(double x) : lkm(x) {}
    double fuel_needed(double d) const override { return lkm * d; }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& config) {
    if (kind == "CAR") {
        double v; config >> v;
        return std::unique_ptr<Vehicle>(new Car(v));
    }
    if (kind == "TRUCK") {
        double v; config >> v;
        return std::unique_ptr<Vehicle>(new Truck(v));
    }
    if (kind == "BIKE")
        return std::unique_ptr<Vehicle>(new Bike());
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    std::vector<double> kms;
    std::string kind;
    while (std::cin >> kind) {
        auto v = create_vehicle(kind, std::cin);
        if (!v) {
            std::string junk; std::getline(std::cin, junk);
            std::cout << "ERROR\n";
            continue;
        }
        double d; std::cin >> d;
        vehicles.push_back(std::move(v));
        kms.push_back(d);
    }

    double tot = 0;
    for (size_t i = 0; i < vehicles.size(); ++i) {
        double f = vehicles[i]->fuel_needed(kms[i]);
        tot += f;
        std::cout << "Trip " << i + 1 << ": " << vehicles[i]->type()
                  << " needs " << std::fixed << std::setprecision(2) << f
                  << " liters\n";
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2) << tot << " liters\n";
    return 0;
}
