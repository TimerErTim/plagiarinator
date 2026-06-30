#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
// adapted from classmate submission

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class MotorCar : public Vehicle {
    double burnRate;
public:
    explicit MotorCar(double r) : burnRate(r) {}
    double fuel_needed(double d) const override { return burnRate * d / 100.0; }
    std::string type() const override { return "CAR"; }
};

class BigRig : public Vehicle {
    double burnRate;
public:
    explicit BigRig(double r) : burnRate(r) {}
    double fuel_needed(double d) const override { return burnRate * d; }
    std::string type() const override { return "TRUCK"; }
};

class TwoWheeler : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> makeRide(const std::string& tag, std::istream& cfg) {
    if (tag == "CAR") { double x; cfg >> x; return std::make_unique<MotorCar>(x); }
    if (tag == "TRUCK") { double x; cfg >> x; return std::make_unique<BigRig>(x); }
    if (tag == "BIKE") return std::make_unique<TwoWheeler>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> ridePool;
    std::vector<double> legKm;
    for (std::string raw; std::getline(std::cin, raw);) {
        std::istringstream parser(raw);
        std::string tag;
        parser >> tag;
        auto ride = makeRide(tag, parser);
        if (!ride) { std::cout << "ERROR\n"; continue; }
        double km; parser >> km;
        ridePool.push_back(std::move(ride));
        legKm.push_back(km);
    }
    double fuelAcc = 0;
    for (std::size_t n = 0; n < ridePool.size(); ++n) {
        double f = ridePool[n]->fuel_needed(legKm[n]);
        fuelAcc += f;
        std::cout << "Trip " << n+1 << ": " << ridePool[n]->type()
                  << " needs " << std::fixed << std::setprecision(2) << f << " liters\n";
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2) << fuelAcc << " liters\n";
    return 0;
}
