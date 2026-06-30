#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include <utility>

class Sensor {
    double celsiusReading;
    std::vector<std::pair<std::string, std::function<void(double)>>> callbacks;

    void dispatch(double value) const {
        for (std::size_t i = 0; i < callbacks.size(); i++) {
            const auto& pair = callbacks[i];
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << pair.first << "] temp=" << value << '\n';
            if (pair.second)
                pair.second(value);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        callbacks.push_back({name, std::move(callback)});
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

    std::string token;
    while (input >> token) {
        if (token == "END") break;
        if (token == "SUB") {
            std::string name;
            input >> name;
            sensor.subscribe(name, [](double) {});
        } else if (token == "SET") {
            double temp;
            input >> temp;
            sensor.set_value(temp);
        }
    }
    return 0;
}
