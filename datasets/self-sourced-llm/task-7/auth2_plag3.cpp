#include <vector>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
// borrowed from lab partner

using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::getline;
using std::istringstream;
using std::istream;
using std::make_unique;
using std::move;
using std::setprecision;
using std::string;
using std::unique_ptr;
using std::vector;
using size_t = std::size_t;

class Vehicle
{
public:
    virtual ~Vehicle() {}
    virtual double fuel_needed(double km) const = 0;
    virtual string type() const = 0;
};

class AutoUnit : public Vehicle
{
    double burnRate;
public:
    AutoUnit(double r): burnRate(r) {}
    double fuel_needed(double km) const override { return burnRate * km / 100.0; }
    string type() const override { return "CAR"; }
};

class HaulerUnit : public Vehicle
{
    double burnRate;
public:
    HaulerUnit(double r): burnRate(r) {}
    double fuel_needed(double km) const override { return burnRate * km; }
    string type() const override { return "TRUCK"; }
};

class PedalUnit : public Vehicle
{
public:
    double fuel_needed(double) const override { return 0.0; }
    string type() const override { return "BIKE"; }
};

unique_ptr<Vehicle> buildRide(const string& tag, istream& specStream)
{
    if (tag == "CAR") {
        double x; specStream >> x;
        return make_unique<AutoUnit>(x);
    }
    if (tag == "TRUCK") {
        double x; specStream >> x;
        return make_unique<HaulerUnit>(x);
    }
    if (tag == "BIKE")
        return make_unique<PedalUnit>();
    return nullptr;
}

int main()
{
    vector<unique_ptr<Vehicle>> rideDeck;
    vector<double> legKm;
    string rawLine;
    while (getline(cin, rawLine)) {
        istringstream parser(rawLine);
        string tag; parser >> tag;
        auto unit = buildRide(tag, parser);
        if (!unit) { cout << "ERROR" << endl; continue; }
        double leg; parser >> leg;
        rideDeck.push_back(move(unit));
        legKm.push_back(leg);
    }
    double accLiters = 0;
    for (size_t slot = 0; slot < rideDeck.size(); ++slot) {
        double need = rideDeck[slot]->fuel_needed(legKm[slot]);
        accLiters += need;
        cout << "Trip " << slot + 1 << ": " << rideDeck[slot]->type()
             << " needs " << fixed << setprecision(2) << need << " liters" << endl;
    }
    cout << "TOTAL " << fixed << setprecision(2) << accLiters << " liters" << endl;
    return 0;
}
