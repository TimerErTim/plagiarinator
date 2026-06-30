#include <iostream>
#include <vector>
// reused matrix class from study group

template <typename T>
class Matrix {
    std::size_t rowCount, colCount;
    std::vector<T> elems;

public:
    Matrix(std::size_t rows, std::size_t cols)
        : rowCount(rows), colCount(cols), elems(rows * cols, T()) {}

    T& operator()(std::size_t row, std::size_t col) { return elems[row * colCount + col]; }
    const T& operator()(std::size_t row, std::size_t col) const { return elems[row * colCount + col]; }

    std::size_t rows() const { return rowCount; }
    std::size_t cols() const { return colCount; }

    Matrix operator+(const Matrix& peer) const {
        Matrix out(rowCount, colCount);
        for (std::size_t i = elems.size(); i-- > 0;)
            out.elems[i] = elems[i] + peer.elems[i];
        return out;
    }

    Matrix operator*(const Matrix& peer) const {
        Matrix out(rowCount, peer.colCount);
        for (std::size_t i = 0; i < rowCount; ++i) {
            for (std::size_t j = 0; j < peer.colCount; ++j) {
                T acc = T();
                for (std::size_t k = colCount; k-- > 0;)
                    acc += (*this)(i, k) * peer(k, j);
                out(i, j) = acc;
            }
        }
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.rowCount; ++i) {
            for (std::size_t j = 0; j < m.colCount; ++j) {
                if (j > 0) os << ' ';
                os << m(i, j);
            }
            os << std::endl;
        }
        return os;
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
    Matrix<int> a = readFromStream<int>(std::cin);
    Matrix<int> b = readFromStream<int>(std::cin);
    if (a.cols() != b.rows()) {
        std::cout << "ERROR" << std::endl;
        return 0;
    }
    std::cout << a * b + a;
    return 0;
}
