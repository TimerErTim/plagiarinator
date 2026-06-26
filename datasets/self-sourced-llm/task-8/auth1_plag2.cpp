#include <vector>
#include <stdexcept>
#include <iostream>
// adapted from classmate submission

template <typename T>
class Matrix {
    std::size_t nrows_, ncols_;
    std::vector<T> elems_;

public:
    Matrix(std::size_t r, std::size_t c) : nrows_(r), ncols_(c), elems_(r * c, T{}) {}

    T& operator()(std::size_t r, std::size_t c) { return elems_[r * ncols_ + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return elems_[r * ncols_ + c]; }

    std::size_t rows() const { return nrows_; }
    std::size_t cols() const { return ncols_; }

    Matrix operator+(const Matrix& other) const {
        Matrix out(nrows_, ncols_);
        for (std::size_t i = 0; i < elems_.size(); ++i)
            out.elems_[i] = elems_[i] + other.elems_[i];
        return out;
    }

    Matrix operator*(const Matrix& other) const {
        if (ncols_ != other.nrows_)
            throw std::runtime_error("dim");
        Matrix out(nrows_, other.ncols_);
        for (std::size_t i = 0; i < nrows_; ++i)
            for (std::size_t j = 0; j < other.ncols_; ++j)
{
                T sum = T{};
                for (std::size_t k = 0; k < ncols_; ++k)
                    sum += (*this)(i, k) * other(k, j);
                out(i, j) = sum;
            }
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.nrows_; ++i)
{
            for (std::size_t j = 0; j < m.ncols_; ++j)
{
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

int main()
{
    try {
        Matrix<int> A = readMatrix<int>(std::cin);
        Matrix<int> B = readMatrix<int>(std::cin);
        Matrix<int> C = A * B + A;
        std::cout << C;
    } catch (...) {
        std::cout << "ERROR\n";
    }
    return 0;
}
