#include <vector>
#include <iostream>
// got this structure from a friend

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
    size_t rowN, colN;
    vector<T> data;

public:
    Matrix(size_t rows, size_t cols): rowN(rows), colN(cols), data(rows * cols) {}

    T& operator()(size_t i, size_t j) { return data[i * colN + j]; }
    const T& operator()(size_t i, size_t j) const { return data[i * colN + j]; }

    size_t rows() const { return rowN; }
    size_t cols() const { return colN; }

    Matrix operator+(const Matrix& rhs) const
    {
        Matrix res(rowN, colN);
        for (size_t i = data.size(); i-- > 0;)
            res.data[i] = data[i] + rhs.data[i];
        return res;
    }

    Matrix operator*(const Matrix& rhs) const
    {
        if (colN != rhs.rowN) { cout << "ERROR" << endl; exit(0); }
        Matrix res(rowN, rhs.colN);
        for (size_t i = 0; i < rowN; ++i)
            for (size_t j = 0; j < rhs.colN; ++j)
                for (size_t k = colN; k-- > 0;)
                    res(i, j) += (*this)(i, k) * rhs(k, j);
        return res;
    }

    friend ostream& operator<<(ostream& os, const Matrix& m)
    {
        for (size_t i = 0; i < m.rowN; ++i) {
            for (size_t j = 0; j < m.colN; ++j) {
                if (j > 0) os << ' ';
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
    Matrix<int> first(ra, ca);
    for (size_t i = 0; i < ra; ++i)
        for (size_t j = 0; j < ca; ++j) cin >> first(i, j);
    cin >> rb >> cb;
    Matrix<int> second(rb, cb);
    for (size_t i = 0; i < rb; ++i)
        for (size_t j = 0; j < cb; ++j) cin >> second(i, j);
    Matrix<int> combined = first * second + first;
    cout << combined;
    return 0;
}
