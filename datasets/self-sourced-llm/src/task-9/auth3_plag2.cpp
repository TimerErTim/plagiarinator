#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

class Sensor {
    double currentValue_;
    std::vector<std::pair<std::string, std::function<void(double)>>> subscribers_;

    void printLine(const std::string& name, double value) const
    {
        std::cout << std::fixed << std::setprecision(1)
                  << "[" << name << "] temp=" << value << std::endl;
    }

    void notifyAll()
    {
        for (const auto& entry : subscribers_)
        {
            printLine(entry.first, currentValue_);
            if (entry.second)
                entry.second(currentValue_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback)
    {
        subscribers_.push_back({name, std::move(callback)});
    }

    void set_value(double celsius)
    {
        currentValue_ = celsius;
        notifyAll();
    }

    double value() const { return currentValue_; }
};

int main()
{
    Sensor temperatureSensor;
    std::string command;
    while (std::cin >> command)
    {
        if (command == "END") break;
        if (command == "SUB")
        {
            std::string observerName;
            std::cin >> observerName;
            temperatureSensor.subscribe(observerName, [](double) {});
        }
        else if (command == "SET")
        {
            double reading;
            std::cin >> reading;
            temperatureSensor.set_value(reading);
        }
    }
    return 0;
}
