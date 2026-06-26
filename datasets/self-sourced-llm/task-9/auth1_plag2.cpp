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

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        observers_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        temp_ = celsius;
        for (const auto& sub : observers_)
{
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << sub.first << "] temp=" << temp_ << '\n';
            if (sub.second) sub.second(temp_);
        }
    }

    double value() const { return temp_; }
};

int main()
{
    Sensor s;
    std::string op;
    while (std::cin >> op)
{
        if (op == "END") break;
        if (op == "SUB")
{
            std::string name;
            std::cin >> name;
            s.subscribe(name, [](double) {});
        } else if (op == "SET") {
            double v;
            std::cin >> v;
            s.set_value(v);
        }
    }
    return 0;
}
