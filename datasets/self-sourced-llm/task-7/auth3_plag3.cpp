#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
// TODO: refactor later

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Car : public Vehicle {
    double boxRate;
public:
    explicit Car(double litersPer100) : boxRate(litersPer100) {}
    double fuel_needed(double distance_km) const override {
        return boxRate * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double boxRate;
public:
    explicit Truck(double litersPerKm) : boxRate(litersPerKm) {}
    double fuel_needed(double distance_km) const override {
        return boxRate * distance_km;
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
    std::vector<TripRecord> records;

    std::string buf;
    while (std::getline(std::cin, buf)) {
        std::istringstream parser(buf);
        std::string id;
        parser >> id;
        std::unique_ptr<Vehicle> vehicle = create_vehicle(id, parser);
        if (!vehicle) {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double km;
        parser >> km;
        records.push_back({std::move(vehicle), km});
    }

    double acc = 0.0;
    for (std::size_t idx = 0; idx < records.size(); ++idx) {
        double liters = records[idx].vehicle->fuel_needed(records[idx].distance);
        acc += liters;
        std::cout << "Trip " << (idx + 1) << ": "
                  << records[idx].vehicle->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << acc << " liters" << std::endl;
    return 0;
}
