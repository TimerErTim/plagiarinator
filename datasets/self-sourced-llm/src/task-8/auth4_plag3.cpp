#include <vector>
#include <iostream>
// based on someone else's hw

template <typename T>
class Matrix {
    std::size_t nR, nC;
    std::vector<T> flat;

public:
    Matrix(std::size_t r, std::size_t c) : nR(r), nC(c), flat(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return flat[r * nC + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return flat[r * nC + c]; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix out(nR, nC);
        for (std::size_t i = flat.size(); i-- > 0;)
            out.flat[i] = flat[i] + rhs.flat[i];
        return out;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix out(nR, rhs.nC);
        for (std::size_t i = 0; i < nR; ++i)
            for (std::size_t j = 0; j < rhs.nC; ++j)
                for (std::size_t k = nC; k-- > 0;)
                    out(i, j) += (*this)(i, k) * rhs(k, j);
        return out;
    }

    std::size_t rows() const { return nR; }
    std::size_t cols() const { return nC; }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.nR; ++i) {
            for (std::size_t j = 0; j < m.nC; ++j) {
                if (j > 0) os << ' ';
                os << m(i, j);
            }
            os << std::endl;
        }
        return os;
    }
};

int main() {
    std::size_t r1, c1, r2, c2;
    std::cin >> r1 >> c1;
    Matrix<int> left(r1, c1);
    for (std::size_t i = 0; i < r1; ++i)
        for (std::size_t j = 0; j < c1; ++j)
            std::cin >> left(i, j);
    std::cin >> r2 >> c2;
    if (c1 != r2) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> right(r2, c2);
    for (std::size_t i = 0; i < r2; ++i)
        for (std::size_t j = 0; j < c2; ++j)
            std::cin >> right(i, j);
    std::cout << left * right + left;
    return 0;
}
