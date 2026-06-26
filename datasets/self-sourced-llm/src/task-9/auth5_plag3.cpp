#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
// TODO: refactor later

class Sensor {
    double body{0.0};
    std::vector<std::pair<std::string, std::function<void(double)>>> pool;

public:
    void subscribe(const std::string& name, std::function<void(double)> cb) {
        pool.emplace_back(name, std::move(cb));
    }

    void set_value(double celsius) {
        body = celsius;
        for (const auto& o : pool) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << o.first << "] temp=" << body << '\n';
            if (o.second) o.second(body);
        }
    }

    double value() const { return body; }
};

int main() {
    Sensor dev;
    std::string c;
    while (std::cin >> c) {
        if (c == "END") break;
        if (c == "SUB") {
            std::string n; std::cin >> n;
            dev.subscribe(n, [](double) {});
        } else if (c == "SET") {
            double hold; std::cin >> hold;
            dev.set_value(hold);
        }
    }
    return 0;
}
