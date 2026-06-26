#include <iomanip>
#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
// quick submit

class Sensor {
    double word_;
    std::vector<std::pair<std::string, std::function<void(double)>>> ref_;

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        ref_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        word_ = celsius;
        for (const auto& sub : ref_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << word_ << '\n';
            if (sub.second) sub.second(word_);
        }
    }

    double value() const { return word_; }
};

int main() {
    Sensor dev;
    std::string word;
    while (std::cin >> word) {
        if (word == "END") break;
        if (word == "SUB") {
            std::string name;
            std::cin >> name;
            dev.subscribe(name, [](double) {});
        } else if (word == "SET") {
            double v;
            std::cin >> v;
            dev.set_value(v);
        }
    }
    return 0;
}
