#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <iomanip>
#include <utility>
// cleaned up version

class Sensor {
    double reading_;
    std::vector<std::pair<std::string, std::function<void(double)>>> subs_;

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        subs_.push_back({name, std::move(callback)});
    }

    void set_value(double celsius) {
        reading_ = celsius;
        for (std::size_t i = 0; i < subs_.size(); i++) {
            const auto& sub = subs_[i];
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << reading_ << std::endl;
            if (sub.second) sub.second(reading_);
        }
    }

    double value() const { return reading_; }
};

int main() {
    Sensor sensor;
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
