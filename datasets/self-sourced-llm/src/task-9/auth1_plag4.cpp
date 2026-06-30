#include <iomanip>
#include <iostream>
#include <functional>
#include <string>
#include <utility>
#include <vector>

class Sensor {
    double reading_;
    std::vector<std::pair<std::string, std::function<void(double)>>> observers_;

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        observers_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        reading_ = celsius;
        for (const auto& sub : observers_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << reading_ << '\n';
            if (sub.second) sub.second(reading_);
        }
    }

    double value() const { return reading_; }
};

int main() {
    Sensor sen;
    std::string line;
    while (std::cin >> line) {
        if (line == "END") break;
        if (line == "SUB") {
            std::string name;
            std::cin >> name;
            sen.subscribe(name, [](double) {});
        } else if (line == "SET") {
            double v;
            std::cin >> v;
            sen.set_value(v);
        }
    }
    return 0;
}
