#include <iomanip>
#include <functional>
#include <string>
#include <iostream>
#include <vector>
// quick submit

using std::cin;
using std::cout;
using std::endl;
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
        observers.emplace_back(name, move(cb));
    }

    void set_value(double celsius)
    {
        val = celsius;
        for (auto& ob : observers) {
            cout << fixed << setprecision(1)
                 << "[" << ob.first << "] temp=" << val << endl;
            if (ob.second) ob.second(val);
        }
    }

    double value() const { return val; }
};

int main()
{
    Sensor s;
    string word;
    while (cin >> word) {
        if (word == "END") break;
        if (word == "SUB") {
            string n; cin >> n;
            s.subscribe(n, [](double) {});
        } else if (word == "SET") {
            double t; cin >> t;
            s.set_value(t);
        }
    }
    return 0;
}
