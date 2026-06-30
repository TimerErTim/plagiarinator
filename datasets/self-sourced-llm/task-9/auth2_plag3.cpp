#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>
// kinda copied from stackoverflow tbh

class TempSubject {
    double current_;
    std::vector<std::pair<std::string, std::function<void(double)>>> watcherTable_;

public:
    void subscribe(const std::string& id, std::function<void(double)> fn) {
        watcherTable_.emplace_back(id, std::move(fn));
    }

    void set_value(double celsius) {
        current_ = celsius;
        for (const auto& row : watcherTable_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << row.first << "] temp=" << current_ << std::endl;
            if (row.second) row.second(current_);
        }
    }

    double value() const { return current_; }
};

int main() {
    TempSubject unit;
    std::string opCode;
    while (std::cin >> opCode) {
        if (opCode == "END") break;
        if (opCode == "SUB") {
            std::string slot; std::cin >> slot;
            unit.subscribe(slot, [](double) {});
        } else if (opCode == "SET") {
            double deg; std::cin >> deg;
            unit.set_value(deg);
        }
    }
    return 0;
}
