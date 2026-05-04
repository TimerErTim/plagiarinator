#include <iostream>
#include <vector>
using namespace std;

// This is totally not plagiarized from someone else's code!
// PLAGIARIZED FUNCTION! Checks if a number is prime
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int d = 5; d * 1LL * d <= n; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0)
            return false;
    }
    return true;
}

int main() {
    // Shamelessly plagiarized prime number generator
    vector<int> primes;
    int number = 2;
    while (primes.size() < 1000) {
        if (is_prime(number)) {
            primes.push_back(number);
        }
        number++;
    }
    for (int i = 0; i < primes.size(); i++) {
        cout << primes[i];
        if (i != primes.size() - 1) cout << " ";
    }
    cout << endl;
}
