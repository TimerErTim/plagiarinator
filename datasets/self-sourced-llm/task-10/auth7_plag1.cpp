#include <vector>
#include <iostream>
#include <string>

class IntBST {
    struct N { int v; N* a; N* b; N(int x):v(x),a(0),b(0){} };
    N* rk;
    void del(N* n){if(n){del(n->a);del(n->b);delete n;}}
    N* add(N* n,int x){if(!n)return new N(x);if(x<n->v)n->a=add(n->a,x);else if(x>n->v)n->b=add(n->b,x);return n;}
    bool chk(const N* n,int x)const{if(!n)return false;if(x==n->v)return true;return x<n->v?chk(n->a,x):chk(n->b,x);}
    void lst(const N* n,std::vector<int>& o)const{if(!n)return;lst(n->a,o);o.emplace_back(n->v);lst(n->b,o);}
    N* mn(N* n)const{while(n&&n->a)n=n->a;return n;}
public:
    IntBST():rk(0){} ~IntBST(){del(rk);}
    void insert(int x){rk=add(rk,x);}
    bool contains(int x)const{return chk(rk,x);}
    void inorder(std::vector<int>& o)const{lst(rk,o);}
    int& min(){return mn(rk)->v;}
    const int& min()const{return mn(rk)->v;}
    bool empty()const{return!rk;}
};

int main(){
    IntBST t; std::string tok;
    while(std::cin>>tok){if(tok=="DONE")break;t.insert(std::stoi(tok));}
    while(std::cin>>tok){
        if(tok=="DONE")break;
        if(tok=="CONTAINS"){int x;std::cin>>x;std::cout<<(t.contains(x)?"YES":"NO")<<std::endl;}
        else if(tok=="MIN"){std::cout<<(t.empty()?"EMPTY":std::to_string(t.min()))<<std::endl;}
        else if(tok=="PRINT"){std::vector<int> v;t.inorder(v);for(size_t i=0;i<v.size();i++){if(i)std::cout<<' ';std::cout<<v[i];}std::cout<<std::endl;}
    }
    return 0;
}
