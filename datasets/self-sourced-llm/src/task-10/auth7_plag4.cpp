#include <string>
#include <iostream>
#include <vector>
// quick submit

class IntBST {
    struct N { int v; N* a; N* b; N(int x):v(x),a(0),b(0){} };
    N* r;
    void del(N* n){if(n){del(n->a);del(n->b);delete n;}}
    N* add(N* n,int x){if(!n)return new N(x);if(x<n->v)n->a=add(n->a,x);else if(x>n->v)n->b=add(n->b,x);return n;}
    bool chk(const N* n,int x)const{if(!n)return false;if(x==n->v)return true;return x<n->v?chk(n->a,x):chk(n->b,x);}
    void lst(const N* n,std::vector<int>& o)const{if(!n)return;lst(n->a,o);o.push_back(n->v);lst(n->b,o);}
    N* mn(N* n)const{while(n&&n->a)n=n->a;return n;}
public:
    IntBST():r(0){} ~IntBST(){del(r);}
    void insert(int x){r=add(r,x);}
    bool contains(int x)const{return chk(r,x);}
    void inorder(std::vector<int>& o)const{lst(r,o);}
    int& min(){return mn(r)->v;}
    const int& min()const{return mn(r)->v;}
    bool empty()const{return!r;}
};

int main(){
    IntBST t; std::string s;
    while(std::cin>>s){if(s=="DONE")break;t.insert(std::stoi(s));}
    while(std::cin>>s){
        if(s=="DONE")break;
        if(s=="CONTAINS"){int x;std::cin>>x;std::cout<<(t.contains(x)?"YES":"NO")<<"\n";}
        else if(s=="MIN"){std::cout<<(t.empty()?"EMPTY":std::to_string(t.min()))<<"\n";}
        else if(s=="PRINT"){std::vector<int> v;t.inorder(v);for(size_t i=0;i<v.size();++i){if(i)std::cout<<' ';std::cout<<v[i];}std::cout<<"\n";}
    }
    return 0;
}
