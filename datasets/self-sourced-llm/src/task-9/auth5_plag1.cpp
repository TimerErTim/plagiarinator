#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class Sensor {
    double reading_{0.0};
    std::vector<std::pair<std::string, std::function<void(double)>>> obs_;

public:
    void subscribe(const std::string& name, std::function<void(double)> cb) {
        obs_.emplace_back(name, std::move(cb));
    }

    void set_value(double celsius) {
        reading_ = celsius;
        for (const auto& o : obs_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << o.first << "] temp=" << reading_ << '\n';
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
