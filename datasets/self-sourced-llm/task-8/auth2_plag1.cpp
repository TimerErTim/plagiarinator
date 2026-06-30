#include <vector>
#include <iostream>

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
    size_t rowCount, colCount;
    vector<T> elems;

public:
    Matrix(size_t rows, size_t cols): rowCount(rows), colCount(cols), elems(rows * cols) {}

    T& operator()(size_t i, size_t j) { return elems[i * colCount + j]; }
    const T& operator()(size_t i, size_t j) const { return elems[i * colCount + j]; }

    size_t rows() const { return rowCount; }
    size_t cols() const { return colCount; }

    Matrix operator+(const Matrix& o) const
    {
        Matrix out(rowCount, colCount);
        for (size_t i = 0; i < elems.size(); i++) out.elems[i] = elems[i] + o.elems[i];
        return out;
    }

    Matrix operator*(const Matrix& o) const
    {
        if (colCount != o.rowCount) { cout << "ERROR" << endl; exit(0); }
        Matrix out(rowCount, o.colCount);
        for (size_t i = 0; i < rowCount; ++i)
            for (size_t j = 0; j < o.colCount; ++j)
                for (size_t k = 0; k < colCount; ++k)
                    out(i, j) += (*this)(i, k) * o(k, j);
        return out;
    }

    friend ostream& operator<<(ostream& os, const Matrix& m)
    {
        for (size_t i = 0; i < m.rowCount; ++i) {
            for (size_t j = 0; j < m.colCount; ++j) {
                if (j) os << ' ';
                os << m(i, j);
            }
            os << endl;
        }
        return os;
    }
};

int main()
{
    size_t ra, ca, rb, cb;
    cin >> ra >> ca;
    Matrix<int> A(ra, ca);
    for (size_t i = 0; i < ra; i++)
        for (size_t j = 0; j < ca; j++) cin >> A(i, j);
    cin >> rb >> cb;
    Matrix<int> B(rb, cb);
    for (size_t i = 0; i < rb; i++)
        for (size_t j = 0; j < cb; j++) cin >> B(i, j);
    Matrix<int> C = A * B + A;
    cout << C;
    return 0;
}
