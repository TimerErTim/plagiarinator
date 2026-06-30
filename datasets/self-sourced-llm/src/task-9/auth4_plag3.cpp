#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>
#include <string>
// found this online and modified it a bit

class HeatMonitor {
    double celsius_;
    std::vector<std::pair<std::string, std::function<void(double)>>> slots_;

public:
    void subscribe(const std::string& name, std::function<void(double)> fn) {
        slots_.push_back({name, fn});
    }

    void set_value(double celsius) {
        celsius_ = celsius;
        for (auto& slot : slots_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << slot.first << "] temp=" << celsius_ << '\n';
            slot.second(celsius_);
        }
    }

    double value() const { return celsius_; }
};

int main() {
    HeatMonitor monitor;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "SUB") {
            std::string n; std::cin >> n;
            monitor.subscribe(n, [](double) {});
        } else if (op == "SET") {
            double v; std::cin >> v;
            monitor.set_value(v);
        }
    }
    return 0;
}
