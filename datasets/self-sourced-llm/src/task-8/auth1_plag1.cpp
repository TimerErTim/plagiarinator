#include <vector>
#include <stdexcept>
#include <iostream>

template <typename T>
class Matrix {
    std::size_t height_, width_;
    std::vector<T> cells_;

public:
    Matrix(std::size_t r, std::size_t c) : height_(r), width_(c), cells_(r * c, T{}) {}

    T& operator()(std::size_t r, std::size_t c) { return cells_[r * width_ + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return cells_[r * width_ + c]; }

    std::size_t rows() const { return height_; }
    std::size_t cols() const { return width_; }

    Matrix operator+(const Matrix& other) const {
        Matrix out(height_, width_);
        for (std::size_t i = 0; i < cells_.size(); i++)
            out.cells_[i] = cells_[i] + other.cells_[i];
        return out;
    }

    Matrix operator*(const Matrix& other) const {
        if (width_ != other.height_)
            throw std::runtime_error("dim");
        Matrix out(height_, other.width_);
        for (std::size_t i = 0; i < height_; ++i)
            for (std::size_t j = 0; j < other.width_; ++j) {
                T sum = T{};
                for (std::size_t k = 0; k < width_; ++k)
                    sum += (*this)(i, k) * other(k, j);
                out(i, j) = sum;
            }
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.height_; ++i) {
            for (std::size_t j = 0; j < m.width_; ++j) {
                if (j) os << ' ';
                os << m(i, j);
            }
            os << std::endl;
        }
        return os;
    }
};

template <typename T>
Matrix<T> loadMatrix(std::istream& in) {
    std::size_t r, c;
    in >> r >> c;
    Matrix<T> mat(r, c);
    for (std::size_t i = 0; i < r; ++i)
        for (std::size_t j = 0; j < c; ++j)
            in >> mat(i, j);
    return mat;
}

int main() {
    try {
        Matrix<int> A = loadMatrix<int>(std::cin);
        Matrix<int> B = loadMatrix<int>(std::cin);
        Matrix<int> C = A * B + A;
        std::cout << C;
    } catch (...) {
        std::cout << "ERROR" << std::endl;
    }
    return 0;
}
