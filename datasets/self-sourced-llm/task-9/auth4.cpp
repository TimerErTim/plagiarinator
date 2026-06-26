#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Sensor {
    double temp;
    std::vector<std::pair<std::string, std::function<void(double)>>> list;

public:
    void subscribe(const std::string& name, std::function<void(double)> fn) {
        list.push_back({name, fn});
    }

    void set_value(double celsius) {
        temp = celsius;
        for (auto& p : list) {
            std::cout << std::fixed << std::setprecision(1)
                      << "[" << p.first << "] temp=" << temp << '\n';
            p.second(temp);
        }
    }

    double value() const { return temp; }
};

int main() {
    Sensor s;
    std::string op;
    while (std::cin >> op) {
        if (op == "END") break;
        if (op == "SUB") {
            std::string n; std::cin >> n;
            s.subscribe(n, [](double) {});
        } else if (op == "SET") {
            double v; std::cin >> v;
            s.set_value(v);
        }
    }
    return 0;
}
