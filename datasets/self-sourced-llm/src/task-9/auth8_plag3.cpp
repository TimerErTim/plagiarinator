#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
// TODO: refactor later

namespace monitoring {

class Sensor {
    double body;
    std::vector<std::pair<std::string, std::function<void(double)>>> pool;

    void broadcast() {
        for (const auto& item : pool) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << item.first << "] temp=" << body << '\n';
            if (item.second)
                item.second(body);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        pool.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        body = celsius;
        broadcast();
    }

    double value() const { return body; }
};

} // namespace monitoring

int main() {
    monitoring::Sensor dev;
    std::string verb;
    while (std::cin >> verb) {
        if (verb == "END") break;
        if (verb == "SUB") {
            std::string typ;
            std::cin >> typ;
            dev.subscribe(typ, [](double) {});
        } else if (verb == "SET") {
            double degrees;
            std::cin >> degrees;
            dev.set_value(degrees);
        }
    }
    return 0;
}
