#include <iostream>
#include <sstream>
#include <vector>

template <typename T>
class Matrix {
    std::size_t nRows, nCols;
    std::vector<T> flat;

public:
    Matrix(std::size_t r, std::size_t c) : nRows(r), nCols(c), flat(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return flat[r * nCols + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return flat[r * nCols + c]; }

    std::size_t rows() const { return nRows; }
    std::size_t cols() const { return nCols; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix out(nRows, nCols);
        for (std::size_t i = 0; i < flat.size(); ++i)
            out.flat[i] = flat[i] + rhs.flat[i];
        return out;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix out(nRows, rhs.nCols);
        for (std::size_t i = 0; i < nRows; ++i)
        {
            for (std::size_t j = 0; j < rhs.nCols; ++j)
            {
                T acc = T{};
                for (std::size_t k = 0; k < nCols; ++k)
                    acc += (*this)(i, k) * rhs(k, j);
                out(i, j) = acc;
            }
        }
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.nRows; ++i)
        {
            for (std::size_t j = 0; j < m.nCols; ++j)
            {
                if (j) os << ' ';
                os << m(i, j);
            }
            os << '\n';
        }
        return os;
    }
};

int main() {
    std::stringstream input;
    input << std::cin.rdbuf();

    std::size_t rA, cA, rB, cB;
    input >> rA >> cA;
    Matrix<int> A(rA, cA);
    for (std::size_t j = 0; j < cA; ++j)
        for (std::size_t i = 0; i < rA; ++i)
            input >> A(i, j);

    input >> rB >> cB;
    if (cA != rB) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> B(rB, cB);
    for (std::size_t i = 0; i < rB; ++i)
        for (std::size_t j = 0; j < cB; ++j)
            input >> B(i, j);

    std::cout << A * B + A;
    return 0;
}
