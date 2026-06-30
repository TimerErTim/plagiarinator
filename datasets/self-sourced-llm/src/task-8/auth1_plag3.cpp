#include <vector>
#include <iostream>
#include <stdexcept>
// tweaked from lab partner code

template <typename T>
class Matrix {
    std::size_t nRows, nCols;
    std::vector<T> flat;

public:
    Matrix(std::size_t r, std::size_t c) : nRows(r), nCols(c), flat(r * c, T()) {}

    T& operator()(std::size_t r, std::size_t c) { return flat[r * nCols + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return flat[r * nCols + c]; }

    std::size_t rows() const { return nRows; }
    std::size_t cols() const { return nCols; }

    Matrix operator+(const Matrix& peer) const {
        Matrix tmp(nRows, nCols);
        for (std::size_t idx = 0; idx < flat.size(); ++idx)
            tmp.flat[idx] = flat[idx] + peer.flat[idx];
        return tmp;
    }

    Matrix operator*(const Matrix& peer) const {
        if (nCols != peer.nRows)
            throw std::runtime_error("dim");
        Matrix tmp(nRows, peer.nCols);
        for (std::size_t i = 0; i < nRows; ++i) {
            for (std::size_t j = 0; j < peer.nCols; ++j) {
                T partial = T{};
                for (std::size_t k = nCols; k-- > 0;)
                    partial += (*this)(i, k) * peer(k, j);
                tmp(i, j) = partial;
            }
        }
        return tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.nRows; ++i) {
            for (std::size_t j = 0; j < m.nCols; ++j) {
                if (j > 0) os << ' ';
                os << m(i, j);
            }
            os << std::endl;
        }
        return os;
    }
};

template <typename T>
Matrix<T> readMatrix(std::istream& in) {
    std::size_t r, c;
    in >> r >> c;
    Matrix<T> m(r, c);
    for (std::size_t i = 0; i < r; ++i)
        for (std::size_t j = 0; j < c; ++j)
            in >> m(i, j);
    return m;
}

int main() {
    try {
        Matrix<int> left = readMatrix<int>(std::cin);
        Matrix<int> right = readMatrix<int>(std::cin);
        Matrix<int> ans = left * right + left;
        std::cout << ans;
    } catch (...) {
        std::cout << "ERROR\n";
    }
    return 0;
}
