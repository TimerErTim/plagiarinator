#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
// modified from tutorial code

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class SedanRide : public Vehicle {
    double burnPer100;
public:
    explicit SedanRide(double litersPer100) : burnPer100(litersPer100) {}
    double fuel_needed(double distance_km) const override {
        return burnPer100 * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class CargoRide : public Vehicle {
    double burnPerKm;
public:
    explicit CargoRide(double litersPerKm) : burnPerKm(litersPerKm) {}
    double fuel_needed(double distance_km) const override {
        return burnPerKm * distance_km;
    }
    std::string type() const override { return "TRUCK"; }
};

class CycleRide : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> spawnRide(const std::string& tag, std::istream& cfg) {
    if (tag == "CAR") {
        double param; cfg >> param;
        return std::make_unique<SedanRide>(param);
    }
    if (tag == "TRUCK") {
        double param; cfg >> param;
        return std::make_unique<CargoRide>(param);
    }
    if (tag == "BIKE")
        return std::make_unique<CycleRide>();
    return nullptr;
}

int main() {
    struct LegEntry {
        std::unique_ptr<Vehicle> ride;
        double km;
    };
    std::vector<LegEntry> ledger;

    std::string rawRow;
    while (std::getline(std::cin, rawRow)) {
        std::istringstream rowParser(rawRow);
        std::string tag;
        rowParser >> tag;
        std::unique_ptr<Vehicle> ride = spawnRide(tag, rowParser);
        if (!ride) {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double km;
        rowParser >> km;
        ledger.push_back({std::move(ride), km});
    }

    double fuelSum = 0.0;
    for (std::size_t leg = 0; leg < ledger.size(); ++leg) {
        double liters = ledger[leg].ride->fuel_needed(ledger[leg].km);
        fuelSum += liters;
        std::cout << "Trip " << (leg + 1) << ": "
                  << ledger[leg].ride->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << fuelSum << " liters" << std::endl;
    return 0;
}
