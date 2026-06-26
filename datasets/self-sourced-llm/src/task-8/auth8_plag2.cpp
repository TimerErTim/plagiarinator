#include <vector>
#include <sstream>
#include <iostream>
// adapted from classmate submission

template <typename T>
class Matrix {
    std::size_t nrows_;
    std::size_t ncols_;
    std::vector<T> elems_;

public:
    Matrix(std::size_t r, std::size_t c) : nrows_(r), ncols_(c), elems_(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return elems_[r * ncols_ + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return elems_[r * ncols_ + c]; }

    std::size_t rows() const { return nrows_; }
    std::size_t cols() const { return ncols_; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix out(nrows_, ncols_);
        for (std::size_t i = 0; i < elems_.size(); ++i)
            out.elems_[i] = elems_[i] + rhs.elems_[i];
        return out;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix out(nrows_, rhs.ncols_);
        for (std::size_t i = 0; i < nrows_; ++i)
            for (std::size_t j = 0; j < rhs.ncols_; ++j)
                for (std::size_t k = 0; k < ncols_; ++k)
                    out(i, j) += (*this)(i, k) * rhs(k, j);
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

int main()
{
    std::stringstream in;
    in << std::cin.rdbuf();

    std::size_t rA, cA, rB, cB;
    in >> rA >> cA;
    Matrix<int> A(rA, cA);
    for (std::size_t i = 0; i < rA; ++i)
        for (std::size_t j = 0; j < cA; ++j)
            in >> A(i, j);

    in >> rB >> cB;
    if (cA != rB)
{
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
