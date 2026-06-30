#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <iomanip>
#include <utility>

class Sensor {
    double tempReading_;
    std::vector<std::pair<std::string, std::function<void(double)>>> listeners_;

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        listeners_.push_back({name, std::move(callback)});
    }

    void set_value(double celsius) {
        tempReading_ = celsius;
        for (std::size_t i = 0; i < listeners_.size(); i++) {
            const auto& sub = listeners_[i];
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << tempReading_ << std::endl;
            if (sub.second) sub.second(tempReading_);
        }
    }

    double value() const { return tempReading_; }
};

int main() {
    Sensor sensor;
    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "SUB") {
            std::string name;
            std::cin >> name;
            sensor.subscribe(name, [](double) {});
        } else if (cmd == "SET") {
            double v;
            std::cin >> v;
            sensor.set_value(v);
        }
    }
    return 0;
}
