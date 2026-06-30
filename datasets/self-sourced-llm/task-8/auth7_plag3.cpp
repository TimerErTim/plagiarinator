#include <sstream>
#include <vector>
#include <iostream>
// saw similar code online, rewrote variable names

template <typename T>
class Matrix {
    std::size_t height, width;
    std::vector<T> cells;

public:
    Matrix(std::size_t r, std::size_t c) : height(r), width(c), cells(r * c) {}

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
        Matrix tmp(height, peer.width);
        for (std::size_t i = 0; i < height; ++i)
            for (std::size_t j = 0; j < peer.width; ++j)
                for (std::size_t k = width; k-- > 0;)
                    tmp(i, j) += (*this)(i, k) * peer(k, j);
        return tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.height; ++i) {
            for (std::size_t j = 0; j < m.width; ++j) {
                if (j > 0) os << ' ';
                os << m(i, j);
            }
            os << std::endl;
        }
        return os;
    }
};

int main() {
    std::stringstream buf;
    buf << std::cin.rdbuf();

    std::size_t rA, cA, rB, cB;
    buf >> rA >> cA;
    Matrix<int> lhs(rA, cA);
    for (std::size_t i = 0; i < rA; ++i)
        for (std::size_t j = 0; j < cA; ++j)
            buf >> lhs(i, j);

    buf >> rB >> cB;
    if (cA != rB) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> rhs(rB, cB);
    for (std::size_t i = 0; i < rB; ++i)
        for (std::size_t j = 0; j < cB; ++j)
            buf >> rhs(i, j);

    std::cout << lhs * rhs + lhs;
    return 0;
}
