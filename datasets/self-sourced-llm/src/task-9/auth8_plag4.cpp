#include <iomanip>
#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
// quick submit

namespace monitoring {

class Sensor {
    double word_;
    std::vector<std::pair<std::string, std::function<void(double)>>> ref_;

    void broadcast() {
        for (const auto& item : ref_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << item.first << "] temp=" << word_ << '\n';
            if (item.second)
                item.second(word_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        ref_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        word_ = celsius;
        broadcast();
    }

    double value() const { return word_; }
};

} // namespace monitoring

int main() {
    monitoring::Sensor dev;
    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "SUB") {
            std::string label;
            std::cin >> label;
            dev.subscribe(label, [](double) {});
        } else if (verb == "SET") {
            double degrees;
            std::cin >> degrees;
            dev.set_value(degrees);
        }
    }
    return 0;
}
