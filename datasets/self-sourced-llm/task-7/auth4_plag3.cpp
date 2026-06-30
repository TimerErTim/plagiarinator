#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
// copied from stackoverflow answer

class Vehicle {
public:
    virtual ~Vehicle() {}
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class RoadCar : public Vehicle {
    double per100;
public:
    RoadCar(double x) : per100(x) {}
    double fuel_needed(double d) const override { return per100 * d / 100.0; }
    std::string type() const override { return "CAR"; }
};

class FreightTruck : public Vehicle {
    double perKm;
public:
    FreightTruck(double x) : perKm(x) {}
    double fuel_needed(double d) const override { return perKm * d; }
    std::string type() const override { return "TRUCK"; }
};

class PushBike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> spawnUnit(const std::string& tag, std::istream& cfg) {
    if (tag == "CAR") {
        double v; cfg >> v;
        return std::unique_ptr<Vehicle>(new RoadCar(v));
    }
    if (tag == "TRUCK") {
        double v; cfg >> v;
        return std::unique_ptr<Vehicle>(new FreightTruck(v));
    }
    if (tag == "BIKE")
        return std::unique_ptr<Vehicle>(new PushBike());
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> unitList;
    std::vector<double> legDist;
    std::string tag;
    while (std::cin >> tag) {
        auto unit = spawnUnit(tag, std::cin);
        if (!unit) {
            std::string junk; std::getline(std::cin, junk);
            std::cout << "ERROR\n";
            continue;
        }
        double d; std::cin >> d;
        unitList.push_back(std::move(unit));
        legDist.push_back(d);
    }

    double fuelSum = 0;
    for (size_t slot = 0; slot < unitList.size(); ++slot) {
        double f = unitList[slot]->fuel_needed(legDist[slot]);
        fuelSum += f;
        std::cout << "Trip " << slot + 1 << ": " << unitList[slot]->type()
                  << " needs " << std::fixed << std::setprecision(2) << f
                  << " liters\n";
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2) << fuelSum << " liters\n";
    return 0;
}
