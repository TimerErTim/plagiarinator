#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <string>
#include <iomanip>
// modified from tutorial code

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Car : public Vehicle {
    double consRate;
public:
    explicit Car(double l100) : consRate(l100) {}
    double fuel_needed(double km) const override { return consRate * km / 100.0; }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double perKm;
public:
    explicit Truck(double lkm) : perKm(lkm) {}
    double fuel_needed(double km) const override { return perKm * km; }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& cfg) {
    if (kind == "CAR") {
        double rate; cfg >> rate;
        return std::make_unique<Car>(rate);
    }
    if (kind == "TRUCK") {
        double rate; cfg >> rate;
        return std::make_unique<Truck>(rate);
    }
    if (kind == "BIKE")
        return std::make_unique<Bike>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> tripVehicles;
    std::vector<double> tripKm;
    std::string row;
    while (std::getline(std::cin, row)) {
        if (row.empty()) continue;
        std::istringstream parser(row);
        std::string kindToken;
        parser >> kindToken;
        auto v = create_vehicle(kindToken, parser);
        if (!v) {
            std::cout << "ERROR\n";
            continue;
        }
        double km;
        parser >> km;
        tripVehicles.push_back(std::move(v));
        tripKm.push_back(km);
    }

    double grandTotal = 0.0;
    for (std::size_t n = 0; n < tripVehicles.size(); ++n) {
        double liters = tripVehicles[n]->fuel_needed(tripKm[n]);
        grandTotal += liters;
        std::cout << "Trip " << (n + 1) << ": " << tripVehicles[n]->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters\n";
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << grandTotal << " liters\n";
    return 0;
}
