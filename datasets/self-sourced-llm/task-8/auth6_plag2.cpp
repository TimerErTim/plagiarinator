#include <iostream>
#include <vector>

template<typename T>
class Matrix {
    std::size_t rows, cols;
    std::vector<T> data;

    T dot(const Matrix& o, std::size_t i, std::size_t j) const {
        T s = T{};
        for (std::size_t k = 0; k < cols; ++k)
            s += (*this)(i, k) * o(k, j);
        return s;
    }

public:
    Matrix(std::size_t x, std::size_t y): rows(x), cols(y), data(x * y) {}

    T& operator()(std::size_t i, std::size_t j) { return data[i * cols + j]; }
    const T& operator()(std::size_t i, std::size_t j) const { return data[i * cols + j]; }

    std::size_t rows_count() const { return rows; }
    std::size_t cols_count() const { return cols; }
    std::size_t rows() const { return rows; }
    std::size_t cols() const { return cols; }

    Matrix operator+(const Matrix& o) const {
        Matrix m(rows, cols);
        for (std::size_t i = 0; i < data.size(); ++i)
            m.data[i] = data[i] + o.data[i];
        return m;
    }

    Matrix operator*(const Matrix& o) const {
        Matrix m(rows, o.cols);
        for (std::size_t i = 0; i < rows; ++i)
            for (std::size_t j = 0; j < o.cols; ++j)
                m(i, j) = dot(o, i, j);
        return m;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.rows; ++i)
        {
            for (std::size_t j = 0; j < m.cols; ++j)
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
    std::size_t ra, ca, rb, cb;
    std::cin >> ra >> ca;
    Matrix<int> A(ra, ca);
    for (std::size_t j = 0; j < ca; ++j)
        for (std::size_t i = 0; i < ra; ++i)
            std::cin >> A(i, j);
    std::cin >> rb >> cb;
    if (ca != rb) { std::cout << "ERROR\n"; return 0; }
    Matrix<int> B(rb, cb);
    for (std::size_t i = 0; i < rb; ++i)
        for (std::size_t j = 0; j < cb; ++j)
            std::cin >> B(i, j);
    std::cout << A * B + A;
    return 0;
}
