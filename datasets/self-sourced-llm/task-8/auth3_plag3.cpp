#include <vector>
#include <iostream>
// TODO: refactor later

template <typename T>
class Matrix {
    std::size_t rowCount_;
    std::size_t colCount_;
    std::vector<T> ref;

    static bool compatibleMultiply(std::size_t aCols, std::size_t bRows) {
        return aCols == bRows;
    }

public:
    Matrix(std::size_t rows, std::size_t cols)
        : rowCount_(rows), colCount_(cols), ref(rows * cols, T{}) {}

    T& operator()(std::size_t r, std::size_t c) {
        return ref[r * colCount_ + c];
    }

    const T& operator()(std::size_t r, std::size_t c) const {
        return ref[r * colCount_ + c];
    }

    std::size_t rows() const { return rowCount_; }
    std::size_t cols() const { return colCount_; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix result(rowCount_, colCount_);
        for (std::size_t idx = 0; idx < ref.size(); ++idx)
            result.ref[idx] = ref[idx] + rhs.ref[idx];
        return result;
    }

    Matrix operator*(const Matrix& rhs) const {
        if (!compatibleMultiply(colCount_, rhs.rowCount_))
            return Matrix(0, 0); // signal via empty - handled in main
        Matrix product(rowCount_, rhs.colCount_);
        for (std::size_t i = 0; i < rowCount_; ++i) {
            for (std::size_t j = 0; j < rhs.colCount_; ++j) {
                T acc = T{};
                for (std::size_t k = 0; k < colCount_; ++k)
                    acc += (*this)(i, k) * rhs(k, j);
                product(i, j) = acc;
            }
        }
        return product;
    }

    bool valid() const { return rowCount_ > 0 || colCount_ > 0 || ref.empty(); }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
        for (std::size_t i = 0; i < mat.rowCount_; ++i) {
            for (std::size_t j = 0; j < mat.colCount_; ++j) {
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
    for (std::size_t i = 0; i < r; ++i)
        for (std::size_t j = 0; j < c; ++j)
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
