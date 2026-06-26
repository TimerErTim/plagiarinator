#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
// adapted from classmate submission

namespace monitoring {

class Sensor {
    double temp_;
    std::vector<std::pair<std::string, std::function<void(double)>>> callbacks_;

    void broadcast() {
        for (const auto& item : callbacks_)
{
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << item.first << "] temp=" << temp_ << '\n';
            if (item.second)
                item.second(temp_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        callbacks_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        temp_ = celsius;
        broadcast();
    }

    double value() const { return temp_; }
};

} // namespace monitoring

int main()
{
    monitoring::Sensor s;
    std::string verb;
    while (std::cin >> verb)
{
        if (verb == "END") break;
        if (verb == "SUB")
{
            std::string kind;
            std::cin >> kind;
            s.subscribe(kind, [](double) {});
        } else if (verb == "SET") {
            double degrees;
            std::cin >> degrees;
            s.set_value(degrees);
        }
    }
    return 0;
}
