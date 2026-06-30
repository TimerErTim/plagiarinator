#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <iomanip>
#include <utility>

class Sensor {
    double reading_{0.0};
    std::vector<std::pair<std::string, std::function<void(double)>>> obs_;

public:
    void subscribe(const std::string& name, std::function<void(double)> cb) {
        obs_.push_back({name, std::move(cb)});
    }

    void set_value(double celsius) {
        reading_ = celsius;
        for (std::size_t i = 0; i < obs_.size(); i++) {
            const auto& o = obs_[i];
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << o.first << "] temp=" << reading_ << std::endl;
            if (o.second) o.second(reading_);
        }
    }

    double value() const { return reading_; }
};

int main() {
    Sensor sen;
    std::string c;
    while (std::cin >> c) {
        if (c == "END") break;
        if (c == "SUB") {
            std::string n; std::cin >> n;
            sen.subscribe(n, [](double) {});
        } else if (c == "SET") {
            double t; std::cin >> t;
            sen.set_value(t);
        }
    }
    return 0;
}
