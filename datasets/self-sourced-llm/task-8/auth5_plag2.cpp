#include <iostream>
#include <vector>

template <typename T>
class Matrix {
    std::size_t nRow, nCol;
    std::vector<T> buf;

public:
    Matrix(std::size_t rows, std::size_t cols)
        : nRow(rows), nCol(cols), buf(rows * cols, T{}) {}

    T& operator()(std::size_t row, std::size_t col) { return buf[row * nCol + col]; }
    const T& operator()(std::size_t row, std::size_t col) const { return buf[row * nCol + col]; }

    std::size_t rows() const { return nRow; }
    std::size_t cols() const { return nCol; }

    Matrix operator+(const Matrix& other) const {
        Matrix sum(nRow, nCol);
        for (std::size_t i = 0; i < buf.size(); ++i)
            sum.buf[i] = buf[i] + other.buf[i];
        return sum;
    }

    Matrix operator*(const Matrix& other) const {
        Matrix product(nRow, other.nCol);
        for (std::size_t i = 0; i < nRow; ++i)
        {
            for (std::size_t j = 0; j < other.nCol; ++j)
            {
                T total = T{};
                for (std::size_t k = 0; k < nCol; ++k)
                    total += (*this)(i, k) * other(k, j);
                product(i, j) = total;
            }
        }
        return product;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix& matrix) {
        for (std::size_t i = 0; i < matrix.nRow; ++i)
        {
            for (std::size_t j = 0; j < matrix.nCol; ++j)
            {
                if (j > 0) stream << ' ';
                stream << matrix(i, j);
            }
            stream << '\n';
        }
        return stream;
    }
};

template <typename T>
Matrix<T> readFromStream(std::istream& in) {
    std::size_t r, c;
    in >> r >> c;
    Matrix<T> m(r, c);
    for (std::size_t j = 0; j < c; ++j)
        for (std::size_t i = 0; i < r; ++i)
            in >> m(i, j);
    return m;
}

int main() {
    Matrix<int> matA = readFromStream<int>(std::cin);
    Matrix<int> matB = readFromStream<int>(std::cin);
    if (matA.cols() != matB.rows()) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> result = matA * matB + matA;
    std::cout << result;
    return 0;
}
