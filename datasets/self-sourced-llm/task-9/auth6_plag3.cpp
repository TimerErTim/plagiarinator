#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <functional>
#include <utility>
#include <string>
// reused code from last semester project

class EventSource {
    double activeCelsius_;
    std::vector<std::pair<std::string, std::function<void(double)>>> listenerQueue_;

    void dispatch(double value) const {
        for (const auto& pair : listenerQueue_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << pair.first << "] temp=" << value << std::endl;
            if (pair.second)
                pair.second(value);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        listenerQueue_.emplace_back(name, std::move(callback));
    }

    void set_value(double celsius) {
        activeCelsius_ = celsius;
        dispatch(activeCelsius_);
    }

    double value() const { return activeCelsius_; }
};

int main() {
    EventSource source;
    std::stringstream input;
    input << std::cin.rdbuf();

    std::string token;
    while (input >> token) {
        if (token == "END") break;
        if (token == "SUB") {
            std::string name;
            input >> name;
            source.subscribe(name, [](double) {});
        } else if (token == "SET") {
            double temp;
            input >> temp;
            source.set_value(temp);
        }
    }
    return 0;
}
