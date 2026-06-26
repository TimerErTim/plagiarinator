#include <vector>
#include <iostream>
// adapted from classmate submission

template <typename T>
class Matrix {
    std::size_t nrows_;
    std::size_t ncols_;
    std::vector<T> elems_;

    static bool compatibleMultiply(std::size_t aCols, std::size_t bRows) {
        return aCols == bRows;
    }

public:
    Matrix(std::size_t rows, std::size_t cols)
        : nrows_(rows), ncols_(cols), elems_(rows * cols, T{}) {}

    T& operator()(std::size_t r, std::size_t c) {
        return elems_[r * ncols_ + c];
    }

    const T& operator()(std::size_t r, std::size_t c) const {
        return elems_[r * ncols_ + c];
    }

    std::size_t rows() const { return nrows_; }
    std::size_t cols() const { return ncols_; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix result(nrows_, ncols_);
        for (std::size_t idx = 0; idx < elems_.size(); ++idx)
            result.elems_[idx] = elems_[idx] + rhs.elems_[idx];
        return result;
    }

    Matrix operator*(const Matrix& rhs) const {
        if (!compatibleMultiply(ncols_, rhs.nrows_))
            return Matrix(0, 0); // signal via empty - handled in main
        Matrix product(nrows_, rhs.ncols_);
        for (std::size_t i = 0; i < nrows_; ++i)
{
            for (std::size_t j = 0; j < rhs.ncols_; ++j)
{
                T acc = T{};
                for (std::size_t k = 0; k < ncols_; ++k)
                    acc += (*this)(i, k) * rhs(k, j);
                product(i, j) = acc;
            }
        }
        return product;
    }

    bool valid() const { return nrows_ > 0 || ncols_ > 0 || elems_.empty(); }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
        for (std::size_t i = 0; i < mat.nrows_; ++i)
{
            for (std::size_t j = 0; j < mat.ncols_; ++j)
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
    for (std::size_t i = 0; i < r; ++i)
        for (std::size_t j = 0; j < c; ++j)
            in >> m(i, j);
    return m;
}

int main()
{
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
