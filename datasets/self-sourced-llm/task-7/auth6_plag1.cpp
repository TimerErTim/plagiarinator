#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
// cleaned up version

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

namespace fleet {

class Car : public Vehicle {
    double litersPer100Km;
public:
    explicit Car(double rate) : litersPer100Km(rate) {}
    double fuel_needed(double distance_km) const override {
        return litersPer100Km * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double litersPerKm;
public:
    explicit Truck(double rate) : litersPerKm(rate) {}
    double fuel_needed(double distance_km) const override {
        return litersPerKm * distance_km;
    }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

} // namespace fleet

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& config) {
    if (kind == "CAR") {
        double rate; config >> rate;
        return std::make_unique<fleet::Car>(rate);
    }
    if (kind == "TRUCK") {
        double rate; config >> rate;
        return std::make_unique<fleet::Truck>(rate);
    }
    if (kind == "BIKE")
        return std::make_unique<fleet::Bike>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    std::vector<double> kms;
    std::string s;

    while (std::getline(std::cin, s)) {
        std::istringstream lineStream(s);
        std::string mode;
        lineStream >> mode;
        std::unique_ptr<Vehicle> vehicle = create_vehicle(mode, lineStream);
        if (!vehicle) {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double distanceKm;
        lineStream >> distanceKm;
        vehicles.emplace_back(std::move(vehicle));
        kms.push_back(distanceKm);
    }

    double total = 0.0;
    for (std::size_t i = 0; i < vehicles.size(); ++i) {
        double liters = vehicles[i]->fuel_needed(kms[i]);
        total += liters;
        std::cout << "Trip " << i + 1 << ": "
                  << vehicles[i]->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << total << " liters" << std::endl;
    return 0;
}
