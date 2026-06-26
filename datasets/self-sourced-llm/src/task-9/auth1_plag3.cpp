#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
// TODO: refactor later

class Sensor {
    double body;
    std::vector<std::pair<std::string, std::function<void(double)>>> pool;

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        pool.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        body = celsius;
        for (const auto& sub : pool) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << body << '\n';
            if (sub.second) sub.second(body);
        }
    }

    double value() const { return body; }
};

int main() {
    Sensor dev;
    std::string line;
    while (std::cin >> line) {
        if (line == "END") break;
        if (line == "SUB") {
            std::string name;
            std::cin >> name;
            dev.subscribe(name, [](double) {});
        } else if (line == "SET") {
            double v;
            std::cin >> v;
            dev.set_value(v);
        }
    }
    return 0;
}
