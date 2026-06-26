#include <iostream>
#include <sstream>
#include <vector>
// cleaned up version

template <typename T>
class Matrix {
    std::size_t rows_;
    std::size_t cols_;
    std::vector<T> data_;

public:
    Matrix(std::size_t r, std::size_t c) : rows_(r), cols_(c), data_(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return data_[r * cols_ + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return data_[r * cols_ + c]; }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix out(rows_, cols_);
        for (std::size_t i = 0; i < data_.size(); ++i)
            out.data_[i] = data_[i] + rhs.data_[i];
        return out;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix out(rows_, rhs.cols_);
        for (std::size_t i = 0; i < rows_; ++i)
            for (std::size_t j = 0; j < rhs.cols_; ++j)
                for (std::size_t k = 0; k < cols_; ++k)
                    out(i, j) += (*this)(i, k) * rhs(k, j);
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
