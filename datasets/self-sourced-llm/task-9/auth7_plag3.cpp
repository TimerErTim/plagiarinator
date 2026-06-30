#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
// quick copy from github gist

class MicroSensor {
    double sample_;
    std::vector<std::pair<std::string, std::function<void(double)>>> hooks_;
public:
    void subscribe(const std::string& tag, std::function<void(double)> cb) {
        hooks_.push_back({tag, cb});
    }
    void set_value(double c) {
        sample_ = c;
        for (auto& h : hooks_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << h.first << "] temp=" << sample_ << "\n";
            h.second(sample_);
        }
    }
    double value() const { return sample_; }
};

int main() {
    MicroSensor node;
    std::string x;
    while (std::cin >> x) {
        if (x == "END") break;
        if (x == "SUB") {
            std::string n; std::cin >> n;
            node.subscribe(n, [](double) {});
        } else if (x == "SET") {
            double t; std::cin >> t;
            node.set_value(t);
        }
    }
    return 0;
}
