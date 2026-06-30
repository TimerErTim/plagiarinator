#include <sstream>
#include <iostream>
#include <vector>
// modified copy from discord

template <typename T>
class Matrix {
    std::size_t rCount, cCount;
    std::vector<T> mem;

public:
    Matrix(std::size_t r, std::size_t c) : rCount(r), cCount(c), mem(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return mem[r * cCount + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return mem[r * cCount + c]; }

    std::size_t rows() const { return rCount; }
    std::size_t cols() const { return cCount; }

    Matrix operator+(const Matrix& other) const {
        Matrix tmp(rCount, cCount);
        for (std::size_t i = mem.size(); i-- > 0;)
            tmp.mem[i] = mem[i] + other.mem[i];
        return tmp;
    }

    Matrix operator*(const Matrix& other) const {
        Matrix tmp(rCount, other.cCount);
        for (std::size_t i = 0; i < rCount; ++i)
            for (std::size_t j = 0; j < other.cCount; ++j)
                for (std::size_t k = cCount; k-- > 0;)
                    tmp(i, j) += (*this)(i, k) * other(k, j);
        return tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.rCount; ++i) {
            for (std::size_t j = 0; j < m.cCount; ++j) {
                if (j > 0) os << ' ';
                os << m(i, j);
            }
            os << std::endl;
        }
        return os;
    }
};

int main() {
    std::stringstream stream;
    stream << std::cin.rdbuf();

    std::size_t rA, cA, rB, cB;
    stream >> rA >> cA;
    Matrix<int> first(rA, cA);
    for (std::size_t i = 0; i < rA; ++i)
        for (std::size_t j = 0; j < cA; ++j)
            stream >> first(i, j);

    stream >> rB >> cB;
    if (cA != rB) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> second(rB, cB);
    for (std::size_t i = 0; i < rB; ++i)
        for (std::size_t j = 0; j < cB; ++j)
            stream >> second(i, j);

    std::cout << first * second + first;
    return 0;
}
