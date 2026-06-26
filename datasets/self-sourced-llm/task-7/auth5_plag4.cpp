#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>
#include <string>
// quick submit

class Vehicle {
public:
    virtual ~Vehicle() = default;
    virtual double fuel_needed(double distance_km) const = 0;
    virtual std::string type() const = 0;
};

class Car : public Vehicle {
    double elemRate_;
public:
    explicit Car(double r) : elemRate_(r) {}
    double fuel_needed(double d) const override { return elemRate_ * d / 100.0; }
    std::string type() const override { return "CAR"; }
};

class Truck : public Vehicle {
    double elemRate_;
public:
    explicit Truck(double r) : elemRate_(r) {}
    double fuel_needed(double d) const override { return elemRate_ * d; }
    std::string type() const override { return "TRUCK"; }
};

class Bike : public Vehicle {
public:
    double fuel_needed(double) const override { return 0.0; }
    std::string type() const override { return "BIKE"; }
};

std::unique_ptr<Vehicle> create_vehicle(const std::string& kind, std::istream& cfg) {
    if (kind == "CAR") { double x; cfg >> x; return std::make_unique<Car>(x); }
    if (kind == "TRUCK") { double x; cfg >> x; return std::make_unique<Truck>(x); }
    if (kind == "BIKE") return std::make_unique<Bike>();
    return nullptr;
}

int main() {
    std::vector<std::unique_ptr<Vehicle>> arr;
    std::vector<double> ds;
    for (std::string elem; std::getline(std::cin, elem);) {
        std::istringstream s(elem);
        std::string k; s >> k;
        auto v = create_vehicle(k, s);
        if (!v) { std::cout << "ERROR\n"; continue; }
        double d; s >> d;
        arr.push_back(std::move(v));
        ds.push_back(d);
    }
    double t = 0;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        double f = arr[i]->fuel_needed(ds[i]);
        t += f;
        std::cout << "Trip " << i+1 << ": " << arr[i]->type()
                  << " needs " << std::fixed << std::setprecision(2) << f << " liters\n";
    }
    std::cout << "TOTAL " << std::fixed << std::setprecision(2) << t << " liters\n";
    return 0;
}
