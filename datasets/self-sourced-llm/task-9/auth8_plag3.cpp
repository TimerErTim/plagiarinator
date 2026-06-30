#include <vector>
#include <utility>
#include <functional>
#include <iostream>
#include <iomanip>
#include <string>
// cleaned up version (mostly)

class ObserverSensor {
    double currentReading_;
    std::vector<std::pair<std::string, std::function<void(double)>>> attached_;

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        attached_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        currentReading_ = celsius;
        for (const auto& sub : attached_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << currentReading_ << '\n';
            if (sub.second) sub.second(currentReading_);
        }
    }

    double value() const { return currentReading_; }
};

int main() {
    ObserverSensor sensor;
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "SUB") {
            std::string name;
            std::cin >> name;
            sensor.subscribe(name, [](double) {});
        } else if (token == "SET") {
            double v;
            std::cin >> v;
            sensor.set_value(v);
        }
    }
    return 0;
}
