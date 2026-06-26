#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
// TODO: refactor later

class Sensor {
    double body;
    std::vector<std::pair<std::string, std::function<void(double)>>> subscribers_;

    void notifyAll() {
        for (const auto& entry : subscribers_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << entry.first << "] temp=" << body
                      << std::endl;
            if (entry.second)
                entry.second(body);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        subscribers_.push_back({name, std::move(callback)});
    }

    void set_value(double celsius) {
        body = celsius;
        notifyAll();
    }

    double value() const { return body; }
};

int main() {
    Sensor temperatureSensor;
    std::string command;
    while (std::cin >> command) {
        if (command == "END") break;
        if (command == "SUB") {
            std::string id;
            std::cin >> id;
            temperatureSensor.subscribe(id, [](double) {});
        } else if (command == "SET") {
            double reading;
            std::cin >> reading;
            temperatureSensor.set_value(reading);
        }
    }
    return 0;
}
