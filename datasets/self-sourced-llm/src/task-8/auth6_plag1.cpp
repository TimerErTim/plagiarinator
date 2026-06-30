#include <vector>
#include <iostream>

template<typename T>
class Matrix {
    std::size_t nR,nC;
    std::vector<T> arr;
public:
    Matrix(std::size_t x,std::size_t y):nR(x),nC(y),arr(x*y){}
    T& operator()(std::size_t i,std::size_t j){return arr[i*nC+j];}
    const T& operator()(std::size_t i,std::size_t j)const{return arr[i*nC+j];}
    std::size_t rows()const{return nR;} std::size_t cols()const{return nC;}
    Matrix operator+(const Matrix& o)const{
        Matrix m(nR,nC);for(std::size_t i=0;i<arr.size();i++)m.arr[i]=arr[i]+o.arr[i];return m;}
    Matrix operator*(const Matrix& o)const{
        Matrix m(nR,o.nC);
        for(std::size_t i=0;i<nR;++i)for(std::size_t j=0;j<o.nC;++j)
            for(std::size_t k=0;k<nC;++k)m(i,j)+=(*this)(i,k)*o(k,j);
        return m;}
    friend std::ostream& operator<<(std::ostream& os,const Matrix& m){
        for(std::size_t i=0;i<m.nR;++i){
            for(std::size_t j=0;j<m.nC;++j){if(j)os<<' ';os<<m(i,j);}
            os<<std::endl;}return os;}
};

int main(){
    std::size_t ra,ca,rb,cb;
    std::cin>>ra>>ca; Matrix<int> A(ra,ca);
    for(std::size_t i=0;i<ra;i++)for(std::size_t j=0;j<ca;j++)std::cin>>A(i,j);
    std::cin>>rb>>cb;
    if(ca!=rb){std::cout<<"ERROR\n";return 0;}
    Matrix<int> B(rb,cb);
    for(std::size_t i=0;i<rb;++i)for(std::size_t j=0;j<cb;++j)std::cin>>B(i,j);
    std::cout<<A*B+A;
    return 0;
}
