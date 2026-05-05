#include <iostream>
#include <vector>

bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * (long long)i <= n; i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main() {
    std::vector<int> primes;
    int candidate = 2;
    while (primes.size() < 1000) {
        if (is_prime(candidate)) {
            primes.push_back(candidate);
        }
        candidate++;
    }
    for (size_t i = 0; i < primes.size(); ++i) {
        std::cout << primes[i];
        if (i != primes.size() - 1)
            std::cout << " ";
    }
    std::cout << std::endl;
    return 0;
}
