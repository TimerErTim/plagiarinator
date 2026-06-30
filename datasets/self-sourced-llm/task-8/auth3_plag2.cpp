#include <iostream>
#include <vector>

template <typename T>
class Matrix {
    std::size_t nR, nC;
    std::vector<T> vals;

    bool dimsOk(std::size_t aCols, std::size_t bRows) const {
        return aCols == bRows;
    }

public:
    Matrix(std::size_t rows, std::size_t cols)
        : nR(rows), nC(cols), vals(rows * cols, T{}) {}

    T& operator()(std::size_t r, std::size_t c) { return vals[r * nC + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return vals[r * nC + c]; }

    std::size_t rows() const { return nR; }
    std::size_t cols() const { return nC; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix result(nR, nC);
        std::size_t n = vals.size();
        for (std::size_t i = 0; i < n; ++i)
            result.vals[i] = vals[i] + rhs.vals[i];
        return result;
    }

    Matrix operator*(const Matrix& rhs) const {
        if (!dimsOk(nC, rhs.nR))
            return Matrix(0, 0);
        Matrix product(nR, rhs.nC);
        for (std::size_t i = 0; i < nR; ++i)
        {
            for (std::size_t j = 0; j < rhs.nC; ++j)
            {
                T acc = T{};
                for (std::size_t k = 0; k < nC; ++k)
                    acc += (*this)(i, k) * rhs(k, j);
                product(i, j) = acc;
            }
        }
        return product;
    }

    bool valid() const { return nR > 0 || nC > 0 || vals.empty(); }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
        for (std::size_t i = 0; i < mat.nR; ++i)
        {
            for (std::size_t j = 0; j < mat.nC; ++j)
            {
                if (j > 0) out << ' ';
                out << mat(i, j);
            }
            out << '\n';
        }
        return out;
    }
};

template <typename T>
Matrix<T> loadMatrix(std::istream& in) {
    std::size_t r, c;
    in >> r >> c;
    Matrix<T> m(r, c);
    for (std::size_t j = 0; j < c; ++j)
        for (std::size_t i = 0; i < r; ++i)
            in >> m(i, j);
    return m;
}

int main() {
    Matrix<int> A = loadMatrix<int>(std::cin);
    Matrix<int> B = loadMatrix<int>(std::cin);
    if (A.cols() != B.rows()) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> C = A * B + A;
    std::cout << C;
    return 0;
}
