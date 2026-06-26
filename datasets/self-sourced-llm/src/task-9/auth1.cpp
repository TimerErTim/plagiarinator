#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Sensor {
    double reading_;
    std::vector<std::pair<std::string, std::function<void(double)>>> subs_;

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        subs_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        reading_ = celsius;
        for (const auto& sub : subs_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << reading_ << '\n';
            if (sub.second) sub.second(reading_);
        }
    }

    double value() const { return reading_; }
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
