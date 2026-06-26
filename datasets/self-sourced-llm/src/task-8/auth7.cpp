#include <iostream>
#include <vector>

template<typename T>
class Matrix {
    std::size_t r,c;
    std::vector<T> v;
public:
    Matrix(std::size_t x,std::size_t y):r(x),c(y),v(x*y){}
    T& operator()(std::size_t i,std::size_t j){return v[i*c+j];}
    const T& operator()(std::size_t i,std::size_t j)const{return v[i*c+j];}
    std::size_t rows()const{return r;} std::size_t cols()const{return c;}
    Matrix operator+(const Matrix& o)const{
        Matrix m(r,c);for(std::size_t i=0;i<v.size();++i)m.v[i]=v[i]+o.v[i];return m;}
    Matrix operator*(const Matrix& o)const{
        Matrix m(r,o.c);
        for(std::size_t i=0;i<r;++i)for(std::size_t j=0;j<o.c;++j)
            for(std::size_t k=0;k<c;++k)m(i,j)+=(*this)(i,k)*o(k,j);
        return m;}
    friend std::ostream& operator<<(std::ostream& os,const Matrix& m){
        for(std::size_t i=0;i<m.r;++i){
            for(std::size_t j=0;j<m.c;++j){if(j)os<<' ';os<<m(i,j);}
            os<<'\n';}return os;}
};

int main(){
    std::size_t ra,ca,rb,cb;
    std::cin>>ra>>ca; Matrix<int> A(ra,ca);
    for(std::size_t i=0;i<ra;++i)for(std::size_t j=0;j<ca;++j)std::cin>>A(i,j);
    std::cin>>rb>>cb;
    if(ca!=rb){std::cout<<"ERROR\n";return 0;}
    Matrix<int> B(rb,cb);
    for(std::size_t i=0;i<rb;++i)for(std::size_t j=0;j<cb;++j)std::cin>>B(i,j);
    std::cout<<A*B+A;
    return 0;
}
