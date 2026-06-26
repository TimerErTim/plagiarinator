#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
// adapted from classmate submission

class Sensor {
    double v;
    std::vector<std::pair<std::string,std::function<void(double)>>> subs;
public:
    void subscribe(const std::string& n,std::function<void(double)> f){subs.push_back({n,f});}
    void set_value(double c){
        v=c;
        for(auto& s:subs)
{
            std::cout<<std::fixed<<std::setprecision(1)
                     <<"["<<s.first<<"] temp="<<v<<"\n";
            s.second(v);
        }
    }
    double value()const{return v;}
};

int main()
{
    Sensor s; std::string x;
    while(std::cin>>x)
{
        if(x=="END")break;
        if(x=="SUB")
{std::string n;std::cin>>n;s.subscribe(n,[](double){});}
        else if(x=="SET")
{double t;std::cin>>t;s.set_value(t);}
    }
    return 0;
}
