#include <vector>
#include <iostream>
// TODO: refactor later

using std::cin;
using std::cout;
using std::endl;
using std::exit;
using std::ostream;
using std::size_t;
using std::vector;

template <typename T>
class Matrix
{
    size_t r, c;
    vector<T> buf;

public:
    Matrix(size_t rows, size_t cols): r(rows), c(cols), buf(rows * cols) {}

    T& operator()(size_t i, size_t j) { return buf[i * c + j]; }
    const T& operator()(size_t i, size_t j) const { return buf[i * c + j]; }

    size_t rows() const { return r; }
    size_t cols() const { return c; }

    Matrix operator+(const Matrix& o) const
    {
        Matrix out(r, c);
        for (size_t i = 0; i < buf.size(); ++i) out.buf[i] = buf[i] + o.buf[i];
        return out;
    }

    Matrix operator*(const Matrix& o) const
    {
        if (c != o.r) { cout << "ERROR" << endl; exit(0); }
        Matrix out(r, o.c);
        for (size_t i = 0; i < r; ++i)
            for (size_t j = 0; j < o.c; ++j)
                for (size_t k = 0; k < c; ++k)
                    out(i, j) += (*this)(i, k) * o(k, j);
        return out;
    }

    friend ostream& operator<<(ostream& os, const Matrix& m)
    {
        for (size_t i = 0; i < m.r; ++i) {
            for (size_t j = 0; j < m.c; ++j) {
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
    size_t ra, ca, rb, cb;
    cin >> ra >> ca;
    Matrix<int> A(ra, ca);
    for (size_t i = 0; i < ra; ++i)
        for (size_t j = 0; j < ca; ++j) cin >> A(i, j);
    cin >> rb >> cb;
    Matrix<int> B(rb, cb);
    for (size_t i = 0; i < rb; ++i)
        for (size_t j = 0; j < cb; ++j) cin >> B(i, j);
    Matrix<int> C = A * B + A;
    cout << C;
    return 0;
}
