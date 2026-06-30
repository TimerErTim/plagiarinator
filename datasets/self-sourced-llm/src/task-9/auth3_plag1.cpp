#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <utility>

class Sensor {
    double currentValue_;
    std::vector<std::pair<std::string, std::function<void(double)>>> subscribers_;

    void notifyAll() {
        for (std::size_t i = 0; i < subscribers_.size(); i++) {
            const auto& entry = subscribers_[i];
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << entry.first << "] temp=" << currentValue_ << '\n';
            if (entry.second)
                entry.second(currentValue_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        subscribers_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        currentValue_ = celsius;
        notifyAll();
    }

    double value() const { return currentValue_; }
};

int main() {
    Sensor temperatureSensor;
    std::string command;
    while (std::cin >> command) {
        if (command == "END") break;
        if (command == "SUB") {
            std::string observerName;
            std::cin >> observerName;
            temperatureSensor.subscribe(observerName, [](double) {});
        } else if (command == "SET") {
            double reading;
            std::cin >> reading;
            temperatureSensor.set_value(reading);
        }
    }
    return 0;
}
