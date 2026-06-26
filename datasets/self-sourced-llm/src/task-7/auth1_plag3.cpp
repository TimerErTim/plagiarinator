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
    double boxCar;
public:
    explicit Car(double l100) : boxCar(l100) {}
    double fuel_needed(double d) const override { return boxCar * d / 100.0; }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double boxTruck;
public:
    explicit Truck(double lkm) : boxTruck(lkm) {}
    double fuel_needed(double d) const override { return boxTruck * d; }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& config) {
    if (kind == "CAR") {
        double rate; config >> rate;
        return std::make_unique<Car>(rate);
    }
    if (kind == "TRUCK") {
        double rate; config >> rate;
        return std::make_unique<Truck>(rate);
    }
    if (kind == "BIKE")
        return std::make_unique<Bike>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> fleet;
    std::vector<double> distances;
    std::string buf;
    while (std::getline(std::cin, buf)) {
        if (buf.empty()) continue;
        std::istringstream ss(buf);
        std::string typ;
        ss >> typ;
        auto veh = create_vehicle(typ, ss);
        if (!veh) {
            std::cout << "ERROR\n";
            continue;
        }
        double dist;
        ss >> dist;
        fleet.push_back(std::move(veh));
        distances.push_back(dist);
    }

    double acc = 0.0;
    for (std::size_t i = 0; i < fleet.size(); ++i) {
        double need = fleet[i]->fuel_needed(distances[i]);
        acc += need;
        std::cout << "Trip " << (i + 1) << ": " << fleet[i]->type()
                  << " needs " << std::fixed << std::setprecision(2) << need
                  << " liters\n";
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2) << acc << " liters\n";
    return 0;
}
