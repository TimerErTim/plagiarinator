#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <functional>
// adapted from classmate submission

class Sensor {
    double tempVal;
    std::vector<std::pair<std::string, std::function<void(double)>>> callbacks;

    void dispatch(double value) const {
        for (const auto& pair : callbacks)
{
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
        tempVal = celsius;
        dispatch(tempVal);
    }

    double value() const { return tempVal; }
};

int main()
{
    Sensor s;
    std::stringstream input;
    input << std::cin.rdbuf();

    std::string op;
    while (input >> op)
{
        if (op == "END") break;
        if (op == "SUB")
{
            std::string name;
            input >> name;
            s.subscribe(name, [](double) {});
        } else if (op == "SET") {
            double temp;
            input >> temp;
            s.set_value(temp);
        }
    }
    return 0;
}
