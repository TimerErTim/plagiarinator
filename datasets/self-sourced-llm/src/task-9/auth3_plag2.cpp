#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
// adapted from classmate submission

class Sensor {
    double temp_;
    std::vector<std::pair<std::string, std::function<void(double)>>> observers_;

    void notifyAll() {
        for (const auto& entry : observers_)
{
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << entry.first << "] temp=" << temp_
                      << std::endl;
            if (entry.second)
                entry.second(temp_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        observers_.push_back({name, std::move(callback)});
    }

    void set_value(double celsius) {
        temp_ = celsius;
        notifyAll();
    }

    double value() const { return temp_; }
};

int main()
{
    Sensor s;
    std::string command;
    while (std::cin >> command)
{
        if (command == "END") break;
        if (command == "SUB")
{
            std::string observerName;
            std::cin >> observerName;
            s.subscribe(observerName, [](double) {});
        } else if (command == "SET") {
            double reading;
            std::cin >> reading;
            s.set_value(reading);
        }
    }
    return 0;
}
