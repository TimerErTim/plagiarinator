#include <vector>
#include <iostream>
// adapted from classmate submission

template <typename T>
class Matrix {
    std::size_t R, C;
    std::vector<T> d;

public:
    Matrix(std::size_t r, std::size_t c) : R(r), C(c), d(r * c, T()) {}

    T& operator()(std::size_t r, std::size_t c) { return d[r * C + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return d[r * C + c]; }

    Matrix operator+(const Matrix& o) const {
        Matrix m(R, C);
        for (std::size_t i = 0; i < d.size(); ++i) m.d[i] = d[i] + o.d[i];
        return m;
    }

    Matrix operator*(const Matrix& o) const {
        Matrix m(R, o.C);
        for (std::size_t i = 0; i < R; ++i)
            for (std::size_t j = 0; j < o.C; ++j)
                for (std::size_t k = 0; k < C; ++k)
                    m(i, j) += (*this)(i, k) * o(k, j);
        return m;
    }

    std::size_t rows() const { return R; }
    std::size_t cols() const { return C; }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.R; ++i)
{
            for (std::size_t j = 0; j < m.C; ++j)
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
    std::size_t ra, ca;
    std::cin >> ra >> ca;
    Matrix<int> A(ra, ca);
    for (std::size_t i = 0; i < ra; ++i)
        for (std::size_t j = 0; j < ca; ++j)
            std::cin >> A(i, j);
    std::size_t rb, cb;
    std::cin >> rb >> cb;
    if (ca != rb)
{ std::cout << "ERROR\n"; return 0; }
    Matrix<int> B(rb, cb);
    for (std::size_t i = 0; i < rb; ++i)
        for (std::size_t j = 0; j < cb; ++j)
            std::cin >> B(i, j);
    std::cout << A * B + A;
    return 0;
}
