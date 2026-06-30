#include <iostream>
#include <vector>

template <typename T>
class Matrix {
    std::size_t rowN, colN;
    std::vector<T> data;

    void readInto(std::istream& in, Matrix& m, std::size_t r, std::size_t c) {
        for (std::size_t j = 0; j < c; ++j)
            for (std::size_t i = 0; i < r; ++i)
                in >> m(i, j);
    }

public:
    Matrix(std::size_t r, std::size_t c) : rowN(r), colN(c), data(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return data[r * colN + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return data[r * colN + c]; }

    Matrix operator+(const Matrix& o) const {
        Matrix m(rowN, colN);
        for (std::size_t i = 0; i < data.size(); ++i)
            m.data[i] = data[i] + o.data[i];
        return m;
    }

    Matrix operator*(const Matrix& o) const {
        Matrix m(rowN, o.colN);
        for (std::size_t i = 0; i < rowN; ++i)
        {
            for (std::size_t j = 0; j < o.colN; ++j)
            {
                T acc = T{};
                for (std::size_t k = 0; k < colN; ++k)
                    acc += (*this)(i, k) * o(k, j);
                m(i, j) = acc;
            }
        }
        return m;
    }

    std::size_t rows() const { return rowN; }
    std::size_t cols() const { return colN; }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.rowN; ++i)
        {
            for (std::size_t j = 0; j < m.colN; ++j)
            {
                if (j) os << ' ';
                os << m(i, j);
            }
            os << '\n';
        }
        return os;
    }
};

int main() {
    std::size_t r1, c1, r2, c2;
    std::cin >> r1 >> c1;
    Matrix<int> A(r1, c1);
    for (std::size_t i = 0; i < r1; ++i)
        for (std::size_t j = 0; j < c1; ++j)
            std::cin >> A(i, j);
    std::cin >> r2 >> c2;
    if (c1 != r2) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> B(r2, c2);
    for (std::size_t i = 0; i < r2; ++i)
        for (std::size_t j = 0; j < c2; ++j)
            std::cin >> B(i, j);
    std::cout << (A * B + A);
    return 0;
}
