#include <iostream>
#include <vector>
using namespace std;

// Checks if 'x' is a prime number
bool is_prime(int x) {
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    for (int j = 3; 1LL * j * j <= x; j += 2)
        if (x % j == 0) return false;
    return true;
}

int main() {
    vector<int> prime_numbers;
    int cnt = 2;
    while (prime_numbers.size() < 1000) {
        if (is_prime(cnt)) {
            prime_numbers.push_back(cnt);
        }
        cnt++;
    }
    for (int k = 0; k < prime_numbers.size(); ++k) {
        cout << prime_numbers[k];
        if (k != (int)prime_numbers.size() - 1) cout << " ";
    }
    cout << "\n";
}