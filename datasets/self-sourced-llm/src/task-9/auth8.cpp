#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace monitoring {

class Sensor {
    double storedValue_;
    std::vector<std::pair<std::string, std::function<void(double)>>> registry_;

    void broadcast() {
        for (const auto& item : registry_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << item.first << "] temp=" << storedValue_ << '\n';
            if (item.second)
                item.second(storedValue_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        registry_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        storedValue_ = celsius;
        broadcast();
    }

    double value() const { return storedValue_; }
};

} // namespace monitoring

int main() {
    monitoring::Sensor probe;
    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "SUB") {
            std::string label;
            std::cin >> label;
            probe.subscribe(label, [](double) {});
        } else if (verb == "SET") {
            double degrees;
            std::cin >> degrees;
            probe.set_value(degrees);
        }
    }
    return 0;
}
