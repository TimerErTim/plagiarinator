#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Matrix {
    std::size_t row_cnt, col_cnt;
    std::vector<T> buf;

    T dotRow(const Matrix& other, std::size_t i, std::size_t j) const {
        T acc = T{};
        for (std::size_t k = 0; k < col_cnt; ++k)
            acc += (*this)(i, k) * other(k, j);
        return acc;
    }

public:
    Matrix(std::size_t r, std::size_t c) : row_cnt(r), col_cnt(c), buf(r * c, T{}) {}

    T& operator()(std::size_t r, std::size_t c) { return buf[r * col_cnt + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return buf[r * col_cnt + c]; }

    std::size_t rows() const { return row_cnt; }
    std::size_t cols() const { return col_cnt; }

    Matrix operator+(const Matrix& other) const {
        Matrix out(row_cnt, col_cnt);
        std::size_t n = buf.size();
        for (std::size_t i = 0; i < n; ++i)
            out.buf[i] = buf[i] + other.buf[i];
        return out;
    }

    Matrix operator*(const Matrix& other) const {
        if (col_cnt != other.row_cnt)
            throw std::runtime_error("dim");
        Matrix out(row_cnt, other.col_cnt);
        for (std::size_t i = 0; i < row_cnt; ++i)
            for (std::size_t j = 0; j < other.col_cnt; ++j)
                out(i, j) = dotRow(other, i, j);
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.row_cnt; ++i)
        {
            for (std::size_t j = 0; j < m.col_cnt; ++j)
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
    for (std::size_t j = 0; j < c; ++j)
        for (std::size_t i = 0; i < r; ++i)
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
