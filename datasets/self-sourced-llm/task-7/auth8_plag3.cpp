#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
// cleaned up version

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class SedanUnit : public Vehicle {
    double consRate;
public:
    explicit SedanUnit(double c) : consRate(c) {}
    double fuel_needed(double distance_km) const override {
        return consRate * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class CargoUnit : public Vehicle {
    double perKmRate;
public:
    explicit CargoUnit(double p) : perKmRate(p) {}
    double fuel_needed(double distance_km) const override {
        return perKmRate * distance_km;
    }
    std::string type() const override { return "TRUCK"; }
};

class PedalUnit : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> spawnFromConfig(const std::string& tag, std::istream& cfg) {
    if (tag == "CAR") {
        double val; cfg >> val;
        return std::make_unique<SedanUnit>(val);
    }
    if (tag == "TRUCK") {
        double val; cfg >> val;
        return std::make_unique<CargoUnit>(val);
    }
    if (tag == "BIKE")
        return std::make_unique<PedalUnit>();
    return nullptr;
}

int main() {
    std::stringstream buf;
    buf << std::cin.rdbuf();

    std::vector<std::unique_ptr<Vehicle>> convoy;
    std::vector<double> legDist;
    std::string raw;
    while (std::getline(buf, raw)) {
        if (raw.empty()) continue;
        std::istringstream rowIn(raw);
        std::string tag;
        rowIn >> tag;
        auto unit = spawnFromConfig(tag, rowIn);
        if (!unit) {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double km;
        rowIn >> km;
        convoy.push_back(std::move(unit));
        legDist.push_back(km);
    }

    double fuelSum = 0.0;
    for (std::size_t leg = 0; leg < convoy.size(); ++leg) {
        double liters = convoy[leg]->fuel_needed(legDist[leg]);
        fuelSum += liters;
        std::cout << "Trip " << leg + 1 << ": " << convoy[leg]->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << fuelSum << " liters" << std::endl;
    return 0;
}
