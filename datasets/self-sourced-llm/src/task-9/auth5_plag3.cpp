#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
// my friend sent me this template

class CelsiusSensor {
    double latestReading_;
    std::vector<std::pair<std::string, std::function<void(double)>>> attached_;

public:
    void subscribe(const std::string& name, std::function<void(double)> cb) {
        attached_.emplace_back(name, std::move(cb));
    }

    void set_value(double celsius) {
        latestReading_ = celsius;
        for (const auto& o : attached_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << o.first << "] temp=" << latestReading_ << '\n';
            if (o.second) o.second(latestReading_);
        }
    }

    double value() const { return latestReading_; }
};

int main() {
    CelsiusSensor device;
    std::string c;
    while (std::cin >> c) {
        if (c == "END") break;
        if (c == "SUB") {
            std::string n; std::cin >> n;
            device.subscribe(n, [](double) {});
        } else if (c == "SET") {
            double t; std::cin >> t;
            device.set_value(t);
        }
    }
    return 0;
}
