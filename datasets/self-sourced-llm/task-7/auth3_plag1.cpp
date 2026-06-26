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

class Car : public Vehicle {
    double consumption_;
public:
    explicit Car(double litersPer100) : consumption_(litersPer100) {}
    double fuel_needed(double distance_km) const override {
        return consumption_ * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double rate_;
public:
    explicit Truck(double litersPerKm) : rate_(litersPerKm) {}
    double fuel_needed(double distance_km) const override {
        return rate_ * distance_km;
    }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& config) {
    if (kind == "CAR") {
        double param; config >> param;
        return std::make_unique<Car>(param);
    } else if (kind == "TRUCK") {
        double param; config >> param;
        return std::make_unique<Truck>(param);
    } else if (kind == "BIKE") {
        return std::make_unique<Bike>();
    }
    return nullptr;
}

int main() {
    struct TripRecord {
        std::unique_ptr<Vehicle> vehicle;
        double distance;
    };
    std::vector<TripRecord> tripLog;

    std::string s;
    while (std::getline(std::cin, s)) {
        std::istringstream parser(s);
        std::string mode;
        parser >> mode;
        std::unique_ptr<Vehicle> vehicle = create_vehicle(mode, parser);
        if (!vehicle) {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double km;
        parser >> km;
        tripLog.push_back({std::move(vehicle), km});
    }

    double total = 0.0;
    for (std::size_t idx = 0; idx < tripLog.size(); ++idx) {
        double liters = tripLog[idx].vehicle->fuel_needed(tripLog[idx].distance);
        total += liters;
        std::cout << "Trip " << (idx + 1) << ": "
                  << tripLog[idx].vehicle->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << total << " liters" << std::endl;
    return 0;
}
