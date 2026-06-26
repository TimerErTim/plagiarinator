#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>
// adapted from classmate submission

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

namespace fleet {

class Car : public Vehicle {
    double lp100;
public:
    explicit Car(double rate) : lp100(rate) {}
    double fuel_needed(double distance_km) const override {
        return lp100 * distance_km / 100.0;
    }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double lpkm;
public:
    explicit Truck(double rate) : lpkm(rate) {}
    double fuel_needed(double distance_km) const override {
        return lpkm * distance_km;
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
    if (kind == "CAR")
{
        double rate; config >> rate;
        return std::make_unique<fleet::Car>(rate);
    }
    if (kind == "TRUCK")
{
        double rate; config >> rate;
        return std::make_unique<fleet::Truck>(rate);
    }
    if (kind == "BIKE")
        return std::make_unique<fleet::Bike>();
    return nullptr;
}

int main()
{
    std::vector<std::unique_ptr<Vehicle>> vs;
    std::vector<double> tripDistances;
    std::string lineBuffer;

    while (std::getline(std::cin, lineBuffer)) {
        std::istringstream lineStream(lineBuffer);
        std::string kindToken;
        lineStream >> kindToken;
        std::unique_ptr<Vehicle> vehicle = create_vehicle(kindToken, lineStream);
        if (!vehicle)
{
            std::cout << "ERROR" << std::endl;
            continue;
        }
        double distanceKm;
        lineStream >> distanceKm;
        vs.emplace_back(std::move(vehicle));
        tripDistances.push_back(distanceKm);
    }

    double totalLiters = 0.0;
    for (std::size_t tripIndex = 0; tripIndex < vs.size(); ++tripIndex) {
        double liters = vs[tripIndex]->fuel_needed(tripDistances[tripIndex]);
        totalLiters += liters;
        std::cout << "Trip " << tripIndex + 1 << ": "
                  << vs[tripIndex]->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << totalLiters << " liters" << std::endl;
    return 0;
}
