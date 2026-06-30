#include <vector>
#include <functional>
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
// borrowed this from someone's homework lol

class ThermometerDevice {
    double storedCelsius_;
    std::vector<std::pair<std::string, std::function<void(double)>>> hookList_;

public:
    void subscribe(const std::string& label, std::function<void(double)> handler) {
        hookList_.emplace_back(label, std::move(handler));
    }

    void set_value(double celsius) {
        storedCelsius_ = celsius;
        for (const auto& hook : hookList_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << hook.first << "] temp=" << storedCelsius_ << '\n';
            if (hook.second) hook.second(storedCelsius_);
        }
    }

    double value() const { return storedCelsius_; }
};

int main() {
    ThermometerDevice probe;
    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "SUB") {
            std::string tag;
            std::cin >> tag;
            probe.subscribe(tag, [](double) {});
        } else if (verb == "SET") {
            double amount;
            std::cin >> amount;
            probe.set_value(amount);
        }
    }
    return 0;
}
