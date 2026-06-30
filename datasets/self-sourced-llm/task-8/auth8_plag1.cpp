#include <vector>
#include <sstream>
#include <iostream>

template <typename T>
class Matrix {
    std::size_t rowLen, colLen;
    std::vector<T> backing;

public:
    Matrix(std::size_t r, std::size_t c) : rowLen(r), colLen(c), backing(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return backing[r * colLen + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return backing[r * colLen + c]; }

    std::size_t rows() const { return rowLen; }
    std::size_t cols() const { return colLen; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix out(rowLen, colLen);
        for (std::size_t i = 0; i < backing.size(); i++)
            out.backing[i] = backing[i] + rhs.backing[i];
        return out;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix out(rowLen, rhs.colLen);
        for (std::size_t i = 0; i < rowLen; ++i)
            for (std::size_t j = 0; j < rhs.colLen; ++j)
                for (std::size_t k = 0; k < colLen; ++k)
                    out(i, j) += (*this)(i, k) * rhs(k, j);
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.rowLen; ++i) {
            for (std::size_t j = 0; j < m.colLen; ++j) {
                if (j) os << ' ';
                os << m(i, j);
            }
            os << std::endl;
        }
        return os;
    }
};

int main() {
    std::stringstream in;
    in << std::cin.rdbuf();

    std::size_t rA, cA, rB, cB;
    in >> rA >> cA;
    Matrix<int> A(rA, cA);
    for (std::size_t i = 0; i < rA; ++i)
        for (std::size_t j = 0; j < cA; ++j)
            in >> A(i, j);

    in >> rB >> cB;
    if (cA != rB) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> B(rB, cB);
    for (std::size_t i = 0; i < rB; ++i)
        for (std::size_t j = 0; j < cB; ++j)
            in >> B(i, j);

    std::cout << A * B + A;
    return 0;
}
