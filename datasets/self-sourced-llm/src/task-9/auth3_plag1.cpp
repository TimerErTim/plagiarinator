#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class Sensor {
    double currentValue_;
    std::vector<std::pair<std::string, std::function<void(double)>>> subscribers_;

    void notifyAll() {
        for (const auto& entry : subscribers_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << entry.first << "] temp=" << currentValue_
                      << std::endl;
            if (entry.second)
                entry.second(currentValue_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        subscribers_.push_back({name, std::move(callback)});
    }

    void set_value(double celsius) {
        currentValue_ = celsius;
        notifyAll();
    }

    double value() const { return currentValue_; }
};

int main() {
    Sensor temperatureSensor;
    std::string token;
    while (std::cin >> token) {
        if (token == "END") break;
        if (token == "SUB") {
            std::string observerName;
            std::cin >> observerName;
            temperatureSensor.subscribe(observerName, [](double) {});
        } else if (token == "SET") {
            double val;
            std::cin >> val;
            temperatureSensor.set_value(val);
        }
    }
    return 0;
}
