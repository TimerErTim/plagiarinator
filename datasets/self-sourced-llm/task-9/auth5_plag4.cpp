#include <iomanip>
#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
// quick submit

class Sensor {
    double word_{0.0};
    std::vector<std::pair<std::string, std::function<void(double)>>> ref_;

public:
    void subscribe(const std::string& name, std::function<void(double)> cb) {
        ref_.emplace_back(name, std::move(cb));
    }

    void set_value(double celsius) {
        word_ = celsius;
        for (const auto& o : ref_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << o.first << "] temp=" << word_ << '\n';
            if (o.second) o.second(word_);
        }
    }

    double value() const { return word_; }
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
