#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
// quick submit before deadline

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

namespace garage {

class AutoVeh : public Vehicle {
    double lPer100;
public:
    explicit AutoVeh(double rate) : lPer100(rate) {}
    double fuel_needed(double distance_km) const override {
        return lPer100 * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class HaulVeh : public Vehicle {
    double lPerKm;
public:
    explicit HaulVeh(double rate) : lPerKm(rate) {}
    double fuel_needed(double distance_km) const override {
        return lPerKm * distance_km;
    }
    std::string type() const override { return "TRUCK"; }
};

class PedVeh : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

} // namespace garage

std::unique_ptr<Vehicle> buildFleetUnit(const std::string& tag, std::istream& cfg) {
    if (tag == "CAR") {
        double rate; cfg >> rate;
        return std::make_unique<garage::AutoVeh>(rate);
    }
    if (tag == "TRUCK") {
        double rate; cfg >> rate;
        return std::make_unique<garage::HaulVeh>(rate);
    }
    if (tag == "BIKE")
        return std::make_unique<garage::PedVeh>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> convoy;
    std::vector<double> legKm;
    std::string buf;

    while (std::getline(std::cin, buf)) {
        std::istringstream rowIn(buf);
        std::string tag;
        rowIn >> tag;
        std::unique_ptr<Vehicle> unit = buildFleetUnit(tag, rowIn);
        if (!unit) {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double km;
        rowIn >> km;
        convoy.emplace_back(std::move(unit));
        legKm.push_back(km);
    }

    double fuelAcc = 0.0;
    for (std::size_t leg = 0; leg < convoy.size(); ++leg) {
        double liters = convoy[leg]->fuel_needed(legKm[leg]);
        fuelAcc += liters;
        std::cout << "Trip " << leg + 1 << ": "
                  << convoy[leg]->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << fuelAcc << " liters" << std::endl;
    return 0;
}
