#include <vector>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
// TODO: refactor later

class Sensor {
    double celsiusVal;
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
        celsiusVal = celsius;
        dispatch(celsiusVal);
    }

    double value() const { return celsiusVal; }
};

int main() {
    Sensor dev;
    std::stringstream input;
    input << std::cin.rdbuf();

    std::string ln;
    while (input >> ln) {
        if (ln == "END") break;
        if (ln == "SUB") {
            std::string name;
            input >> name;
            dev.subscribe(name, [](double) {});
        } else if (ln == "SET") {
            double temp;
            input >> temp;
            dev.set_value(temp);
        }
    }
    return 0;
}
