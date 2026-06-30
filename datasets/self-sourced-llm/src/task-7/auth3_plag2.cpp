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

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& config)
{
    if (kind == "CAR")
    {
        double param; config >> param;
        return std::make_unique<Car>(param);
    }
    if (kind == "TRUCK")
    {
        double param; config >> param;
        return std::make_unique<Truck>(param);
    }
    if (kind == "BIKE")
        return std::make_unique<Bike>();
    return nullptr;
}

bool extract(const std::string& line, std::unique_ptr<Vehicle>& veh, double& dist)
{
    std::istringstream parser(line);
    std::string vehicleKind;
    parser >> vehicleKind;
    veh = create_vehicle(vehicleKind, parser);
    if (!veh)
        return false;
    parser >> dist;
    return true;
}

int main()
{
    struct TripRecord {
        std::unique_ptr<Vehicle> vehicle;
        double distance;
    };
    std::vector<TripRecord> records;

    std::string inputLine;
    while (std::getline(std::cin, inputLine))
    {
        std::unique_ptr<Vehicle> vehicle;
        double km;
        if (!extract(inputLine, vehicle, km))
        {
            std::cout << "ERROR" << std::endl;
            continue;
        }
        records.push_back({std::move(vehicle), km});
    }

    double grandTotal = 0.0;
    for (std::size_t idx = 0; idx < records.size(); ++idx)
    {
        double liters = records[idx].vehicle->fuel_needed(records[idx].distance);
        grandTotal += liters;
        std::cout << "Trip " << (idx + 1) << ": "
                  << records[idx].vehicle->type()
                  << " needs " << std::fixed << std::setprecision(2)
                  << liters << " liters" << std::endl;
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2)
              << grandTotal << " liters" << std::endl;
    return 0;
}
