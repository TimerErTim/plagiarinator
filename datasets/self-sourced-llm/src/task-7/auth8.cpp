#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Car : public Vehicle {
    double consumption_;
public:
    explicit Car(double c) : consumption_(c) {}
    double fuel_needed(double distance_km) const override {
        return consumption_ * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double perKm_;
public:
    explicit Truck(double p) : perKm_(p) {}
    double fuel_needed(double distance_km) const override {
        return perKm_ * distance_km;
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
        double val; config >> val;
        return std::make_unique<Car>(val);
    }
    if (kind == "TRUCK") {
        double val; config >> val;
        return std::make_unique<Truck>(val);
    }
    if (kind == "BIKE")
        return std::make_unique<Bike>();
    return nullptr;
}

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    std::vector<std::unique_ptr<Vehicle>> fleet;
    std::vector<double> distances;
    std::string row;
    while (std::getline(input, row)) {
        if (row.empty()) continue;
        std::istringstream rowStream(row);
        std::string kind;
        rowStream >> kind;
        auto vehicle = create_vehicle(kind, rowStream);
        if (!vehicle) {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double km;
        rowStream >> km;
        fleet.push_back(std::move(vehicle));
        distances.push_back(km);
    }

    double runningTotal = 0.0;
    for (std::size_t n = 0; n < fleet.size(); ++n) {
        double liters = fleet[n]->fuel_needed(distances[n]);
        runningTotal += liters;
        std::cout << "Trip " << n + 1 << ": " << fleet[n]->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << runningTotal << " liters" << std::endl;
    return 0;
}
