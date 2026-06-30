#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Sensor
{
    double val;
    std::vector<std::pair<std::string, std::function<void(double)>>> observers;

    void emit(double celsius)
    {
        for (auto& ob : observers)
        {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << ob.first << "] temp=" << celsius << std::endl;
            if (ob.second)
                ob.second(celsius);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> cb)
    {
        observers.emplace_back(name, std::move(cb));
    }

    void set_value(double celsius)
    {
        val = celsius;
        emit(val);
    }

    double value() const { return val; }
};

int main()
{
    Sensor s;
    std::string cmd;
    while (std::cin >> cmd)
    {
        if (cmd == "END") break;
        if (cmd == "SUB")
        {
            std::string n;
            std::cin >> n;
            s.subscribe(n, [](double) {});
        }
        else if (cmd == "SET")
        {
            double t;
            std::cin >> t;
            s.set_value(t);
        }
    }
    return 0;
}
