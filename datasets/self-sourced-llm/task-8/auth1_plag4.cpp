#include <stdexcept>
#include <vector>
#include <iostream>

template <typename T>
class Matrix {
    std::size_t rows_, cols_;
    std::vector<T> values_;

public:
    Matrix(std::size_t r, std::size_t c) : rows_(r), cols_(c), values_(r * c, T{}) {}

    T& operator()(std::size_t r, std::size_t c) { return values_[r * cols_ + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return values_[r * cols_ + c]; }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    Matrix operator+(const Matrix& other) const {
        Matrix out(rows_, cols_);
        for (std::size_t i = 0; i < values_.size(); ++i)
            out.values_[i] = values_[i] + other.values_[i];
        return out;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols_ != other.rows_)
            throw std::runtime_error("bad dims");
        Matrix out(rows_, other.cols_);
        for (std::size_t i = 0; i < rows_; ++i)
            for (std::size_t j = 0; j < other.cols_; ++j) {
                T sum = T{};
                for (std::size_t k = 0; k < cols_; ++k)
                    sum += (*this)(i, k) * other(k, j);
                out(i, j) = sum;
            }
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.rows_; ++i) {
            for (std::size_t j = 0; j < m.cols_; ++j) {
                if (j) os << ' ';
                os << m(i, j);
            }
            os << '\n';
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
        Matrix<int> M1 = readMatrix<int>(std::cin);
        Matrix<int> M2 = readMatrix<int>(std::cin);
        Matrix<int> Out = M1 * M2 + M1;
        std::cout << Out;
    } catch (...) {
        std::cout << "ERROR\n";
    }
    return 0;
}
