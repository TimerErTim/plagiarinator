#include <iostream>
#include <vector>
#include <sstream>

template <typename T>
class Matrix {
    std::size_t nR, nC;
    std::vector<T> store;

public:
    Matrix(std::size_t r, std::size_t c) : nR(r), nC(c), store(r * c) {}

    T& operator()(std::size_t r, std::size_t c) { return store[r * nC + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return store[r * nC + c]; }

    std::size_t rows() const { return nR; }
    std::size_t cols() const { return nC; }

    Matrix operator+(const Matrix& rhs) const {
        Matrix out(nR, nC);
        for (std::size_t i = 0; i < store.size(); ++i)
            out.store[i] = store[i] + rhs.store[i];
        return out;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix out(nR, rhs.nC);
        for (std::size_t i = 0; i < nR; ++i)
        {
            for (std::size_t j = 0; j < rhs.nC; ++j)
            {
                T acc = T{};
                for (std::size_t k = 0; k < nC; ++k)
                    acc += (*this)(i, k) * rhs(k, j);
                out(i, j) = acc;
            }
        }
        return out;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t i = 0; i < m.nR; ++i)
        {
            for (std::size_t j = 0; j < m.nC; ++j)
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
    std::stringstream in;
    in << std::cin.rdbuf();

    std::size_t rA, cA, rB, cB;
    in >> rA >> cA;
    Matrix<int> A(rA, cA);
    for (std::size_t j = 0; j < cA; ++j)
        for (std::size_t i = 0; i < rA; ++i)
            in >> A(i, j);

    in >> rB >> cB;
    if (cA != rB) {
        std::cout << "ERROR\n";
        return 0;
    }
    Matrix<int> B(rB, cB);
    for (std::size_t i = 0; i < rB; ++i)
        for (std::size_t j = 0; j < cB; ++j)
            in >> B(i, j);

    std::cout << A * B + A;
    return 0;
}
