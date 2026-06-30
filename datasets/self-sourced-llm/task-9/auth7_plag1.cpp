#include <iostream>
#include <iomanip>
#include <functional>
#include <string>
#include <vector>

class Sensor {
    double v;
    std::vector<std::pair<std::string,std::function<void(double)>>> subs;
public:
    void subscribe(const std::string& n,std::function<void(double)> f){subs.emplace_back(n,f);}
    void set_value(double c){
        v=c;
        for(std::size_t i=0;i<subs.size();i++){
            auto& s=subs[i];
            std::cout<<std::fixed<<std::setprecision(1)
                     <<"["<<s.first<<"] temp="<<v<<std::endl;
            s.second(v);
        }
    }
    double value()const{return v;}
};

int main(){
    Sensor s; std::string x;
    while(std::cin>>x){
        if(x=="END")break;
        if(x=="SUB"){std::string n;std::cin>>n;s.subscribe(n,[](double){});}
        else if(x=="SET"){double t;std::cin>>t;s.set_value(t);}
    }
    return 0;
}
