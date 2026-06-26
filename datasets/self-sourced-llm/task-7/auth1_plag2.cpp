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

class Car : public Vehicle {
    double consumption_;
public:
    explicit Car(double l100) : consumption_(l100) {}
    double fuel_needed(double d) const override { return consumption_ * d / 100.0; }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double rate_;
public:
    explicit Truck(double lkm) : rate_(lkm) {}
    double fuel_needed(double d) const override { return rate_ * d; }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& config) {
    if (kind == "CAR")
{
        double rate; config >> rate;
        return std::make_unique<Car>(rate);
    }
    if (kind == "TRUCK")
{
        double rate; config >> rate;
        return std::make_unique<Truck>(rate);
    }
    if (kind == "BIKE")
        return std::make_unique<Bike>();
    return nullptr;
}

int main()
{
    std::vector<std::unique_ptr<Vehicle>> fleet;
    std::vector<double> ds;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string kind;
        ss >> kind;
        auto veh = create_vehicle(kind, ss);
        if (!veh)
{
            std::cout << "ERROR\n";
            continue;
        }
        double dist;
        ss >> dist;
        fleet.push_back(std::move(veh));
        ds.push_back(dist);
    }

    double sum = 0.0;
    for (std::size_t i = 0; i < fleet.size(); ++i) {
        double need = fleet[i]->fuel_needed(ds[i]);
        sum += need;
        std::cout << "Trip " << (i + 1) << ": " << fleet[i]->type()
                  << " needs " << std::fixed << std::setprecision(2) << need
                  << " liters\n";
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2) << sum << " liters\n";
    return 0;
}
