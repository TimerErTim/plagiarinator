#include <iostream>
#include <iomanip>
#include <functional>
#include <utility>
#include <vector>
#include <string>
// adapted from classmate submission

class ReadingHub {
    double liveTemp_;
    std::vector<std::pair<std::string, std::function<void(double)>>> clientList_;

    void notifyAll() {
        for (const auto& entry : clientList_) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << entry.first << "] temp=" << liveTemp_
                      << std::endl;
            if (entry.second)
                entry.second(liveTemp_);
        }
    }

public:
    void subscribe(const std::string& name, std::function<void(double)> callback) {
        clientList_.push_back({name, std::move(callback)});
    }

    void set_value(double celsius) {
        liveTemp_ = celsius;
        notifyAll();
    }

    double value() const { return liveTemp_; }
};

int main() {
    ReadingHub hub;
    std::string command;
    while (std::cin >> command) {
        if (command == "END") break;
        if (command == "SUB") {
            std::string observerName;
            std::cin >> observerName;
            hub.subscribe(observerName, [](double) {});
        } else if (command == "SET") {
            double reading;
            std::cin >> reading;
            hub.set_value(reading);
        }
    }
    return 0;
}
