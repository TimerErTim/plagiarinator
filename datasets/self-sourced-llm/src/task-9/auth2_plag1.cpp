#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::fixed;
using std::function;
using std::move;
using std::pair;
using std::setprecision;
using std::string;
using std::vector;

class Sensor
{
    double val;
    vector<pair<string, function<void(double)>>> observers;

public:
    void subscribe(const string& name, function<void(double)> cb)
    {
        observers.push_back({name, move(cb)});
    }

    void set_value(double celsius)
    {
        val = celsius;
        for (std::size_t i = 0; i < observers.size(); i++) {
            auto& ob = observers[i];
            cout << fixed << setprecision(1)
                 << "[" << ob.first << "] temp=" << val << '\n';
            if (ob.second) ob.second(val);
        }
    }

    double value() const { return val; }
};

int main()
{
    Sensor s;
    string cmd;
    while (cin >> cmd) {
        if (cmd == "END") break;
        if (cmd == "SUB") {
            string n; cin >> n;
            s.subscribe(n, [](double) {});
        } else if (cmd == "SET") {
            double t; cin >> t;
            s.set_value(t);
        }
    }
    return 0;
}
