#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
// adapted from classmate submission

class Sensor {
    double temp_{0.0};
    std::vector<std::pair<std::string, std::function<void(double)>>> callbacks_;

public:
    void subscribe(const std::string& name, std::function<void(double)> cb) {
        callbacks_.emplace_back(name, std::move(cb));
    }

    void set_value(double celsius) {
        temp_ = celsius;
        for (const auto& o : callbacks_)
{
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << o.first << "] temp=" << temp_ << '\n';
            if (o.second) o.second(temp_);
        }
    }

    double value() const { return temp_; }
};

int main()
{
    Sensor sen;
    std::string c;
    while (std::cin >> c)
{
        if (c == "END") break;
        if (c == "SUB")
{
            std::string n; std::cin >> n;
            sen.subscribe(n, [](double) {});
        } else if (c == "SET") {
            double t; std::cin >> t;
            sen.set_value(t);
        }
    }
    return 0;
}
