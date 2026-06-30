#include <cstdio>
#include <cmath>

// Simple function to check for primality of a number
bool check_prime(unsigned int n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    unsigned int limit = (unsigned int)std::sqrt(n);
    for (unsigned int i = 5; i <= limit; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int main() {
    const int desired_primes = 1000;
    int count = 0;
    unsigned int num = 2;
    bool printed_first = false;

    while (count < desired_primes) {
        if (check_prime(num)) {
            if (printed_first) std::printf(" ");
            std::printf("%u", num);
            ++count;
            printed_first = true;
        }
        ++num;
    }
    std::printf("\n");
    return 0;
}