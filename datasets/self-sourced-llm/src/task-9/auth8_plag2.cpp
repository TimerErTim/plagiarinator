#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
// cleaned up version

class Sensor {
    double reading_;
    std::vector<std::pair<std::string, std::function<void(double)>>> subs_;

    void notifyAll(double value) const
    {
        for (const auto& sub : subs_)
        {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << value << '\n';
            if (sub.second)
                sub.second(value);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback)
    {
        subs_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius)
    {
        reading_ = celsius;
        notifyAll(reading_);
    }

    double value() const { return reading_; }
};

int main()
{
    Sensor sensor;
    std::string token;
    while (std::cin >> token)
    {
        if (token == "END") break;
        if (token == "SUB")
        {
            std::string name;
            std::cin >> name;
            sensor.subscribe(name, [](double) {});
        }
        else if (token == "SET")
        {
            double v;
            std::cin >> v;
            sensor.set_value(v);
        }
    }
    return 0;
}
