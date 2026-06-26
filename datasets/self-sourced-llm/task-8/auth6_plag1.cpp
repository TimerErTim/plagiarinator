#include <iostream>
#include <sstream>
#include <vector>
// cleaned up version

template <typename T>
class Matrix {
    std::size_t numRows;
    std::size_t numCols;
    std::vector<T> storage;

public:
    Matrix(std::size_t rows, std::size_t cols)
        : numRows(rows), numCols(cols), storage(rows * cols, T{}) {}

    T& operator()(std::size_t row, std::size_t col) {
        return storage[row * numCols + col];
    }

    const T& operator()(std::size_t row, std::size_t col) const {
        return storage[row * numCols + col];
    }

    std::size_t rows() const { return numRows; }
    std::size_t cols() const { return numCols; }

    Matrix operator+(const Matrix& other) const {
        Matrix sum(numRows, numCols);
        for (std::size_t i = 0; i < storage.size(); ++i)
            sum.storage[i] = storage[i] + other.storage[i];
        return sum;
    }

    Matrix operator*(const Matrix& other) const {
        Matrix product(numRows, other.numCols);
        for (std::size_t i = 0; i < numRows; ++i) {
            for (std::size_t j = 0; j < other.numCols; ++j) {
                T total = T{};
                for (std::size_t k = 0; k < numCols; ++k)
                    total += (*this)(i, k) * other(k, j);
                product(i, j) = total;
            }
        }
        return product;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Matrix& matrix) {
        for (std::size_t i = 0; i < matrix.numRows; ++i) {
            for (std::size_t j = 0; j < matrix.numCols; ++j) {
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
    for (std::size_t i = 0; i < r; ++i)
        for (std::size_t j = 0; j < c; ++j)
            in >> m(i, j);
    return m;
}

int main() {
    Matrix<int> A = readFromStream<int>(std::cin);
    Matrix<int> B = readFromStream<int>(std::cin);
    if (A.cols() != B.rows()) {
        std::cout << "ERROR" << std::endl;
        return 0;
    }
    Matrix<int> C = A * B + A;
    std::cout << C;
    return 0;
}
