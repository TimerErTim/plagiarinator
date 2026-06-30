#include <iostream>
#include <vector>
// same idea as tutorial solution, renamed a bit

template <typename T>
class Matrix {
    std::size_t height, width;
    std::vector<T> cells;

    static bool canMul(std::size_t leftCols, std::size_t rightRows) {
        return leftCols == rightRows;
    }

public:
    Matrix(std::size_t rows, std::size_t cols)
        : height(rows), width(cols), cells(rows * cols, T()) {}

    T& operator()(std::size_t r, std::size_t c) { return cells[r * width + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return cells[r * width + c]; }

    std::size_t rows() const { return height; }
    std::size_t cols() const { return width; }

    Matrix operator+(const Matrix& peer) const {
        Matrix tmp(height, width);
        for (std::size_t i = cells.size(); i-- > 0;)
            tmp.cells[i] = cells[i] + peer.cells[i];
        return tmp;
    }

    Matrix operator*(const Matrix& peer) const {
        if (!canMul(width, peer.height))
            return Matrix(0, 0);
        Matrix tmp(height, peer.width);
        for (std::size_t i = 0; i < height; ++i) {
            for (std::size_t j = 0; j < peer.width; ++j) {
                T total = T();
                for (std::size_t k = width; k-- > 0;)
                    total += (*this)(i, k) * peer(k, j);
                tmp(i, j) = total;
            }
        }
        return tmp;
    }

    bool valid() const { return height > 0 || width > 0 || cells.empty(); }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
        for (std::size_t i = 0; i < mat.height; ++i) {
            for (std::size_t j = 0; j < mat.width; ++j) {
                if (j > 0) out << ' ';
                out << mat(i, j);
            }
            out << std::endl;
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
    Matrix<int> lhs = loadMatrix<int>(std::cin);
    Matrix<int> rhs = loadMatrix<int>(std::cin);
    if (lhs.cols() != rhs.rows()) {
        std::cout << "ERROR\n";
        return 0;
    }
    std::cout << lhs * rhs + lhs;
    return 0;
}
