#include <iostream>
#include <vector>

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
    size_t nRow, nCol;
    vector<T> storage;

public:
    Matrix(size_t rows, size_t cols): nRow(rows), nCol(cols), storage(rows * cols) {}

    T& operator()(size_t i, size_t j) { return storage[i * nCol + j]; }
    const T& operator()(size_t i, size_t j) const { return storage[i * nCol + j]; }

    size_t rows() const { return nRow; }
    size_t cols() const { return nCol; }

    Matrix operator+(const Matrix& o) const
    {
        Matrix out(nRow, nCol);
        for (size_t idx = 0; idx < storage.size(); ++idx)
            out.storage[idx] = storage[idx] + o.storage[idx];
        return out;
    }

    Matrix operator*(const Matrix& o) const
    {
        if (nCol != o.nRow)
        {
            cout << "ERROR" << endl;
            exit(0);
        }
        Matrix out(nRow, o.nCol);
        for (size_t i = 0; i < nRow; ++i)
            for (size_t j = 0; j < o.nCol; ++j)
            {
                T acc = T{};
                for (size_t k = 0; k < nCol; ++k)
                    acc += (*this)(i, k) * o(k, j);
                out(i, j) = acc;
            }
        return out;
    }

    friend ostream& operator<<(ostream& os, const Matrix& m)
    {
        for (size_t i = 0; i < m.nRow; ++i)
        {
            for (size_t j = 0; j < m.nCol; ++j)
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
    size_t ra, ca, rb, cb;
    cin >> ra >> ca;
    Matrix<int> A(ra, ca);
    for (size_t j = 0; j < ca; ++j)
        for (size_t i = 0; i < ra; ++i)
            cin >> A(i, j);
    cin >> rb >> cb;
    Matrix<int> B(rb, cb);
    for (size_t i = 0; i < rb; ++i)
        for (size_t j = 0; j < cb; ++j)
            cin >> B(i, j);
    Matrix<int> C = A * B + A;
    cout << C;
    return 0;
}
