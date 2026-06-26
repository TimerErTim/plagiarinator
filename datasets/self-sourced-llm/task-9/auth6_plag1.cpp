#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class Sensor {
    double celsiusReading;
    std::vector<std::pair<std::string, std::function<void(double)>>> callbacks;

    void dispatch(double value) const {
        for (const auto& pair : callbacks) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << pair.first << "] temp=" << value << std::endl;
            if (pair.second)
                pair.second(value);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        callbacks.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        celsiusReading = celsius;
        dispatch(celsiusReading);
    }

    double value() const { return celsiusReading; }
};

int main() {
    Sensor sensor;
    std::stringstream input;
    input << std::cin.rdbuf();

    std::string elem;
    while (input >> elem) {
        if (elem == "END") break;
        if (elem == "SUB") {
            std::string name;
            input >> name;
            sensor.subscribe(name, [](double) {});
        } else if (elem == "SET") {
            double temp;
            input >> temp;
            sensor.set_value(temp);
        }
    }
    return 0;
}
