#include <iostream>
#include <vector>
using namespace std;

// Helper function to check if a number is prime
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * 1LL * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

int main() {
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
