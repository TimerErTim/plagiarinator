#include <iostream>
#include <vector>

template <typename T>
class Matrix {
    std::size_t nrows, ncols;
    std::vector<T> vals;

public:
    Matrix(std::size_t r, std::size_t c) : nrows(r), ncols(c), vals(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return vals[r * ncols + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return vals[r * ncols + c]; }

    Matrix operator+(const Matrix& o) const {
        Matrix m(nrows, ncols);
        for (std::size_t i = vals.size(); i-- > 0;) m.vals[i] = vals[i] + o.vals[i];
        return m;
    }

    Matrix operator*(const Matrix& o) const {
        Matrix m(nrows, o.ncols);
        for (std::size_t i = 0; i < nrows; ++i)
            for (std::size_t j = 0; j < o.ncols; ++j) {
                T acc = T{};
                for (std::size_t k = 0; k < ncols; ++k)
                    acc += (*this)(i, k) * o(k, j);
                m(i, j) = acc;
            }
        return m;
    }

    std::size_t rows() const { return nrows; }
    std::size_t cols() const { return ncols; }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.nrows; ++i) {
            for (std::size_t j = 0; j < m.ncols; ++j) {
                if (j) os << ' ';
                os << m(i, j);
            }
            os << '\n';
        }
        return os;
    }
};

int main() {
    std::size_t r1, c1, r2, c2;
    std::cin >> r1 >> c1;
    Matrix<int> matA(r1, c1);
    for (std::size_t i = 0; i < r1; ++i)
        for (std::size_t j = 0; j < c1; ++j)
            std::cin >> matA(i, j);
    std::cin >> r2 >> c2;
    if (c1 != r2) { std::cout << "ERROR\n"; return 0; }
    Matrix<int> matB(r2, c2);
    for (std::size_t i = 0; i < r2; ++i)
        for (std::size_t j = 0; j < c2; ++j)
            std::cin >> matB(i, j);
    std::cout << matA * matB + matA;
    return 0;
}
